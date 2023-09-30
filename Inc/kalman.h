#ifndef KALMAN_H
#define KALMAN_H

#include <initializer_list>
#include <algorithm>
#include <cstdint>

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

template <class T, uint16_t size>
class Vector
{
public:
    using Type = T;
    static constexpr uint16_t Size = size;

    Vector() {}

    Vector(std::initializer_list<T> il) {
        std::copy(il.begin(), il.end(), data_);
    }

    T& operator[](uint16_t i) { return data_[i]; }

    T operator[](uint16_t i) const { return data_[i]; }

    Vector<T, size> operator+(const Vector<T, size>& rhs) const
    {
        Vector<T, size> result;
        for (uint16_t i = 0; i < size; ++i) {
            result[i] = data_[i] + rhs[i];
        }
        return result;
    }

    Vector<T, size>& operator+=(const Vector<T, size>& rhs)
    {
        for (uint16_t i = 0; i < size; ++i) {
            data_[i] += rhs[i];
        }
        return *this;
    }

private:
    T data_[size]{};
};

template <class T, uint16_t nrows, uint16_t ncols>
class Matrix
{
public:
    using Type = T;
    static constexpr uint16_t NumRows = nrows;
    static constexpr uint16_t NumCols = ncols;

    Matrix() {}

    Matrix(std::initializer_list<T> il) {
        std::copy(il.begin(), il.end(), data_);
    }

    T& operator()(uint16_t row, uint16_t col) { return data_[row * ncols + col]; }

    T operator()(uint16_t row, uint16_t col) const { return data_[row * ncols + col]; }

    Matrix<T, nrows, ncols> operator+(const Matrix<T, nrows, ncols>& rhs) const
    {
        Matrix<T, nrows, ncols> result;
        for (uint16_t i = 0; i < nrows * ncols; ++i) {
            result.data_[i] = data_[i] + rhs.data_[i];
        }
        return result;
    }

    Matrix<T, nrows, ncols>& operator+=(const Matrix<T, nrows, ncols>& rhs)
    {
        for (uint16_t i = 0; i < nrows * ncols; ++i) {
            data_[i] += rhs.data_[i];
        }
        return *this;
    }

    Vector<T, nrows> operator*(const Vector<T, ncols>& vec) const
    {
        Vector<T, nrows> result;
        for (uint16_t row = 0; row < nrows; ++row) {
            for (uint16_t col = 0; col < ncols; ++col) {
                result[row] += at(row, col) * vec[col];
            }
        }
        return result;
    }


    template <uint16_t size>
    Matrix<T, nrows, size> operator*(const Matrix<T, ncols, size>& mat) const
    {
        Matrix<T, nrows, size> result;
        for (uint16_t row = 0; row < nrows; ++row) {
            for (uint16_t col = 0; col < size; ++col) {
                for (uint16_t i = 0; i < ncols; ++i) {
                    result(row, col) += at(row, i) * mat(i, col);
                }
            }
        }
        return result;
    }

    Matrix<T, ncols, nrows> transpose() const
    {
        Matrix<T, nrows, ncols> result;
        transpose(result);
        return result;
    }

    void transpose(Matrix<T, ncols, nrows>& tr) const
    {
        for (uint16_t row = 0; row < nrows; ++row) {
            for (uint16_t col = 0; col < ncols; ++col) {
                tr(row, col) = at(col, row);
            }
        }
    }

    Matrix<T, nrows, nrows> inverse() const;
    void inverse(Matrix<T, nrows, nrows>& inv) const;

private:
    T& at(uint16_t row, uint16_t col) { return data_[row * ncols + col]; }

    T at(uint16_t row, uint16_t col) const { return data_[row * ncols + col]; }

    T data_[nrows * ncols]{};
};

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
