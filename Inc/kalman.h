#ifndef KALMAN_H
#define KALMAN_H

#include "matrix.h"
#include "vector.h"

namespace mart
{

/*
Kalman Filter estimates the state of a linear discrete process
x_t = Ax_{t-1} + Bu_t + eps_t

To do so it uses not only the process equation but also a measurement
z_t = Cx_t + delta_t

x_t is a considered a gaussian variable with mean mu_t and variance Sigma_t.

Kalman Filter consists of 2 steps:
1) Prediction
mu_prio_t = A * mu_{t-1} + B *u_t
Sigma_priot_t = A*Sigma_t*A^T_t + R_t
R_t is the covariance matrix of the noise eps_t.

2) Correction
Coefficient K_t is introduced to get the optimal Sigma_t
K_t = Sigma_prio_t * (C_t * Sigma_prio_t * C^T_t + Q_t)^{-1}
mu_t = mu_prio_t + K_t * (z_t - C_t*mu_prio_t)
Sigma_t = (I - K_t*C_t) * Sigma_prio_t

For our purposes we'll ignore the control
 */

template <class T, uint16_t stateSize, uint16_t measurementSize>
class KalmanFilter
{
public:
    using ValueType = T;
    using State = Vector<ValueType, stateSize>;
    using Measurement = Vector<ValueType, measurementSize>;
    using ProcessMatrix = Matrix<ValueType, stateSize, stateSize>;
    using MeasurementMatrix = Matrix<ValueType, measurementSize, measurementSize>;

    const State& state() const { return muPost_; }

    void update(const Measurement& z)
    {
        // prediction
        muPrio_ = A_ * muPost_;
        SigmaPrio_ = A_ * SigmaPost_ * AT_ + R_;

        // correction
        K_ = SigmaPrio_ * (C_ * SigmaPrio_ * CT_ + Q_).inverse();
        muPost_ = muPrio_ + K_ * (z - C_ * muPrio_);
        SigmaPost_ = (I_ - K_ * C_) * SigmaPrio_;
    }

private:
    using KalmanMatrix = Matrix<ValueType, stateSize, measurementSize>;

    const ProcessMatrix A_;
    const ProcessMatrix AT_;
    const ProcessMatrix R_;
    const MeasurementMatrix C_;
    const MeasurementMatrix CT_;
    const MeasurementMatrix Q_;
    const ProcessMatrix I_;
    KalmanMatrix K_;

    State muPost_;
    State muPrio_;
    ProcessMatrix SigmaPost_;
    ProcessMatrix SigmaPrio_;
};

}  // namespace mart

#endif /* KALMAN_H */
