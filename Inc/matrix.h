#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <initializer_list>
#include <algorithm>
#include <cstdint>

namespace mart
{

namespace alloc {
template <class T, uint16_t nrows, uint16_t ncols>
class Matrix;
}

template <class T, uint16_t nrows, uint16_t ncols, uint16_t skip=0>
class Matrix
{
public:
    using Type = T;
    static constexpr uint16_t NumRows = nrows;
    static constexpr uint16_t NumCols = ncols;
    using Alloc = alloc::Matrix<T, nrows, ncols>;

    explicit Matrix(T* data) : d_(data) {}

    const T* raw() const { return d_; }

    T& operator()(uint16_t row, uint16_t col) { return at(row, col); }

    T operator()(uint16_t row, uint16_t col) const { return at(row, col); }

    Alloc operator+(const Matrix<T, nrows, ncols>& rhs) const;

    Matrix<T, nrows, ncols>& operator+=(const Matrix<T, nrows, ncols>& rhs);

    alloc::Vector<T, nrows> operator*(const Vector<T, ncols>& vec) const;

    template <uint16_t size>
    alloc::Matrix<T, nrows, size> operator*(const Matrix<T, ncols, size>& mat) const;

    alloc::Matrix<T, ncols, nrows> transpose() const;

    void transpose(Matrix<T, ncols, nrows>& tr) const;

    void luDecompose(Matrix<T, nrows, nrows>& L, Matrix<T, nrows, nrows>& U) const;

    Matrix<T, nrows, nrows> inverse() const;
    void inverse(Matrix<T, nrows, nrows>& inv) const;

    static alloc::Matrix<T, nrows, ncols> eye();

    template<uint16_t subRows, uint16_t subCols>
    Matrix<T, subRows, subCols, ncols + skip - subCols> submat(uint16_t fromRow, uint16_t fromCol);

protected:
    T& at(uint16_t row, uint16_t col) { return d_[row * (ncols + skip) + col]; }

    T at(uint16_t row, uint16_t col) const { return d_[row * (ncols + skip) + col]; }

    T* d_;
};

namespace alloc
{
template <class T, uint16_t nrows, uint16_t ncols>
class Matrix : public ::mart::Matrix<T, nrows, ncols>
{
public:
    Matrix() : ::mart::Matrix<T, nrows, ncols>(data_) {}

    explicit Matrix(const T* data) : Matrix()
    {
        std::copy(data, data + nrows * ncols, data_);
    }

    Matrix(const Matrix<T, nrows, ncols>& other) : Matrix(other.data_) {}

    Matrix(const ::mart::Matrix<T, nrows, ncols>& other) : Matrix(other.raw()) {}

    Matrix(std::initializer_list<T> il) : Matrix()
    {
        std::copy(il.begin(), il.end(), data_);
    }

private:
    T data_[nrows * ncols]{};
};

}  // namespace alloc

template <class T, uint16_t nrows, uint16_t ncols, uint16_t skip>
alloc::Matrix<T, nrows, ncols> Matrix<T, nrows, ncols, skip>::operator+(const Matrix<T, nrows, ncols>& rhs) const
{
    alloc::Matrix<T, nrows, ncols> result;
    for (uint16_t row = 0; row < nrows; ++row) {
        for (uint16_t col = 0; col < ncols; ++col) {
            result(row, col) = at(row, col) + rhs(row, col);
        }
    }
    return result;
}

template <class T, uint16_t nrows, uint16_t ncols, uint16_t skip>
Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols, skip>::operator+=(const Matrix<T, nrows, ncols>& rhs)
{
    for (uint16_t row = 0; row < nrows; ++row) {
        for (uint16_t col = 0; col < ncols; ++col) {
            at(row, col) += rhs(row, col);
        }
    }
    return *this;
}

template <class T, uint16_t nrows, uint16_t ncols, uint16_t skip>
alloc::Vector<T, nrows> Matrix<T, nrows, ncols, skip>::operator*(const Vector<T, ncols>& vec) const
{
    alloc::Vector<T, nrows> result;
    for (uint16_t row = 0; row < nrows; ++row) {
        for (uint16_t col = 0; col < ncols; ++col) {
            result[row] += at(row, col) * vec[col];
        }
    }
    return result;
}

template <class T, uint16_t nrows, uint16_t ncols, uint16_t skip>
template <uint16_t size>
alloc::Matrix<T, nrows, size> Matrix<T, nrows, ncols, skip>::operator*(const Matrix<T, ncols, size>& mat) const
{
    alloc::Matrix<T, nrows, size> result;
    for (uint16_t row = 0; row < nrows; ++row) {
        for (uint16_t col = 0; col < size; ++col) {
            for (uint16_t i = 0; i < ncols; ++i) {
                result(row, col) += at(row, i) * mat(i, col);
            }
        }
    }
    return result;
}

template <class T, uint16_t nrows, uint16_t ncols, uint16_t skip>
alloc::Matrix<T, ncols, nrows> Matrix<T, nrows, ncols, skip>::transpose() const
{
    alloc::Matrix<T, nrows, ncols> result;
    transpose(result);
    return result;
}

template <class T, uint16_t nrows, uint16_t ncols, uint16_t skip>
void Matrix<T, nrows, ncols, skip>::transpose(Matrix<T, ncols, nrows>& tr) const
{
    for (uint16_t row = 0; row < nrows; ++row) {
        for (uint16_t col = 0; col < ncols; ++col) {
            tr(row, col) = at(col, row);
        }
    }
}

template <class T, uint16_t nrows, uint16_t ncols, uint16_t skip>
void Matrix<T, nrows, ncols, skip>::luDecompose(Matrix<T, nrows, nrows>& L, Matrix<T, nrows, nrows>& U) const
{
    for (uint16_t i = 0; i < nrows; ++i) {
        L(i, i) = 1;
    }

    for (uint16_t j = 0; j < nrows; ++j) {
        for (uint16_t i = 0; i <= j; ++i) {
            U(i, j) = at(i, j);
            for (uint16_t k = 0; (k + 1) <= i; ++k) {
                U(i, j) -= L(i, k) * U(k, j);
            }
        }
        for (uint16_t i = j + 1; i < nrows; ++i) {
            L(i, j) = at(i, j);
            for (uint16_t k = 0; (k + 1) <= j; ++k) {
                L(i, j) -= L(i, k) * U(k, j);
            }
            L(i, j) /= U(j, j);
        }
    }
}

template <class T, uint16_t nrows, uint16_t ncols, uint16_t skip>
alloc::Matrix<T, nrows, ncols> Matrix<T, nrows, ncols, skip>::eye()
{
    alloc::Matrix<T, nrows, ncols> result;
    for (uint16_t i = 0; i < nrows; ++i) {
        result(i, i) = 1;
    }
    return result;
}

template <class T, uint16_t nrows, uint16_t ncols, uint16_t skip>
template <uint16_t subRows, uint16_t subCols>
Matrix<T, subRows, subCols, ncols + skip - subCols>
Matrix<T, nrows, ncols, skip>::submat(uint16_t fromRow, uint16_t fromCol)
{
    return Matrix<T, subRows, subCols, ncols + skip - subCols>(&at(fromRow, fromCol));
}

}

#endif /* MATRIX_H */
