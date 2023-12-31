#ifndef EXTKALMAN_H
#define EXTKALMAN_H

#include "matrix.h"
#include "vector.h"
#include <functional>

namespace mart
{

/*
Extended Kalman Filter estimates the state of a nonlinear discrete process
x_t = f(x_{t-1}, u_t) + eps_t

The measurement is also nonlinear in this case:
z_t = h(x_t) + delta_t

To overcome nonlinearity we use Jacobians:
F_t = df(x_{t-1}, u_t) / dx | x_{t_1}
H_t = dh(x_t) / dx | x_prio_t

Extended Kalman Filter also consists of 2 steps:
1) Prediction
mu_prio_t = f(mu_{t-1}, u_t)
Sigma_priot_t = F_t*Sigma_t*F^T_t + R_t
R_t is the covariance matrix of the noise eps_t.

2) Correction
Coefficient K_t is introduced to get the optimal Sigma_t
K_t = Sigma_prio_t * (H_t * Sigma_prio_t * H^T_t + Q_t)^{-1}
mu_t = mu_prio_t + K_t * (z_t - h(mu_prio_t))
Sigma_t = (I - K_t*H_t) * Sigma_prio_t

For our purposes we'll ignore the control
*/

template<class T, uint16_t stateSize, uint16_t measurementSize>
class ExtendedKalmanFilter
{
public:
    using ValueType = T;

    using State = Vector<ValueType, stateSize>;
    using ProcessFunction =
        std::function<void(State&, const State&, ValueType)>;
    using ProcessMatrix = Matrix<ValueType, stateSize, stateSize>;
    using GetProcessJacobianFunction =
        std::function<void(const State&, ProcessMatrix&, ValueType)>;

    using Measurement = Vector<ValueType, measurementSize>;
    using MeasurementFunction = std::function<Measurement(const State&, ValueType)>;
    using MeasurementMatrix = Matrix<ValueType, measurementSize, stateSize>;
    using GetMeasurementJacobianFunction =
        std::function<void(const State&, MeasurementMatrix&, ValueType)>;

    ExtendedKalmanFilter(
        ProcessFunction f,
        GetProcessJacobianFunction getProcessJacobian,
        ProcessMatrix processCovariance,
        MeasurementFunction h,
        GetMeasurementJacobianFunction getMeasurementJacobian,
        MeasurementMatrix measurementCovariance
        ) :
        f_(std::move(f)),
        getProcessJacobian_(std::move(getProcessJacobian)),
        R_(processCovariance),
        h_(std::move(h)),
        getMeasurementJacobian_(std::move(getMeasurementJacobian)),
        Q_(measurementCovariance),
        I_(ProcessMatrix::eye())
    {}


    const State& state() const { return muPost_; }

    void update(const Measurement& z, ValueType dt)
    {
        getProcessJacobian(muPost_, F_, dt);
        F_.transpose(FT_);

        // prediction
        f_(muPrio_, muPost_, dt);
        SigmaPrio_ = F_ * SigmaPost_ * FT_ + R_;

        getMeasurementJacobian(muPrio_, H_, dt);
        H_.transpose(HT_);

        // correction
        K_ = SigmaPrio_ * (H_ * SigmaPrio_ * HT_ + Q_).inverse();
        muPost_ = muPrio_ + K_ * (z - h_(muPrio_));
        SigmaPost_ = (I_ - K_ * H_) * SigmaPrio_;
    }

private:
    using AllocKalmanMatrix = alloc::Matrix<ValueType, stateSize, measurementSize>;
    using AllocState        = typename State::Alloc;
    using AllocProcessMatrix     = typename ProcessMatrix::Alloc;
    using AllocMeasurementMatrix = typename MeasurementMatrix::Alloc;

    const ProcessFunction f_;
    const GetProcessJacobianFunction getProcessJacobian_;
    AllocProcessMatrix F_;
    AllocProcessMatrix FT_;
    const AllocProcessMatrix R_;
    const MeasurementFunction h_;
    const GetMeasurementJacobianFunction getMeasurementJacobian_;
    AllocMeasurementMatrix H_;
    AllocMeasurementMatrix HT_;
    const AllocMeasurementMatrix Q_;
    const AllocProcessMatrix I_;
    AllocKalmanMatrix K_;

    AllocState muPost_;
    AllocState muPrio_;
    AllocProcessMatrix SigmaPost_;
    AllocProcessMatrix SigmaPrio_;
};

}

#endif /* EXTKALMAN_H */
