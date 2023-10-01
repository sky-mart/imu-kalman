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
z_t = Cx_x + delta_t

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
 */

template <class A, class R, class C, class Q>
class KalmanFilter
{
public:
    using Type = typename A::Type;
    using State = Vector<Type, A::NumRows>;
    using Measurement = Vector<Type, C::NumRows>;

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
    using StateCovariance = Matrix<Type, A::NumRows, A::NumRows>;
    using K = Matrix<Type, State::Size, Measurement::Size>;

    const A A_;
    const A AT_;
    const R R_;
    const C C_;
    const C CT_;
    const Q Q_;
    const StateCovariance I_;
    K K_;

    State muPost_;
    State muPrio_;
    StateCovariance SigmaPost_;
    StateCovariance SigmaPrio_;
};

}  // namespace mart

#endif /* KALMAN_H */
