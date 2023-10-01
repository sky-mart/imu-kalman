#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <initializer_list>
#include <algorithm>
#include <cstdint>

namespace mart
{

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

    static Matrix<T, nrows, nrows> eye()
    {
        Matrix<T, nrows, ncols> result;
        for (uint16_t i = 0; i < nrows; ++i) {
            result(i, i) = 1;
        }
    }

private:
    T& at(uint16_t row, uint16_t col) { return data_[row * ncols + col]; }

    T at(uint16_t row, uint16_t col) const { return data_[row * ncols + col]; }

    T data_[nrows * ncols]{};
};

template<class T, uint16_t size>
Matrix<T, size, size> rotationMatrix(const Vector<T, size>& w)
{
    return {
        0,    -w[2], w[1],
        w[2],  0,   -w[0],
        -w[1], w[0], 0
    };
}

}

#endif /* MATRIX_H */
