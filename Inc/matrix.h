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

template <class T, uint16_t nrows, uint16_t ncols>
class Matrix
{
public:
    using Type = T;
    static constexpr uint16_t NumRows = nrows;
    static constexpr uint16_t NumCols = ncols;
    using Alloc = alloc::Matrix<T, nrows, ncols>;

    // Needed to be able to declare an array of matrices
    Matrix() {}

    explicit Matrix(T* data, uint16_t skipCols = 0) : d_(data), skipCols_(skipCols) {}

    Matrix<T, nrows, ncols>& operator=(std::initializer_list<T> il);

    const T* raw() const { return d_; }

    T& operator()(uint16_t row, uint16_t col) { return at(row, col); }

    T operator()(uint16_t row, uint16_t col) const { return at(row, col); }

    Alloc operator+(const Matrix<T, nrows, ncols>& rhs) const;

    Matrix<T, nrows, ncols>& operator+=(const Matrix<T, nrows, ncols>& rhs);

    alloc::Matrix<T, nrows, ncols> operator*(T mul) const;

    Matrix<T, nrows, ncols>& operator*=(T mul) const;

    alloc::Vector<T, nrows> operator*(const Vector<T, ncols>& vec) const;

    template <uint16_t size>
    alloc::Matrix<T, nrows, size> operator*(const Matrix<T, ncols, size>& mat) const;

    alloc::Matrix<T, ncols, nrows> transpose() const;

    void transpose(Matrix<T, ncols, nrows>& tr) const;

    void luDecompose(Matrix<T, nrows, nrows>& L, Matrix<T, nrows, nrows>& U) const;

    Matrix<T, nrows, nrows> inverse() const;
    void inverse(Matrix<T, nrows, nrows>& inv) const;

    static alloc::Matrix<T, nrows, ncols> eye();

    template <uint16_t subRows, uint16_t subCols>
    Matrix<T, subRows, subCols> submat(uint16_t fromRow, uint16_t fromCol);

    template <uint16_t subRows, uint16_t subCols>
    alloc::Matrix<Matrix<T, subRows, subCols>, nrows / subRows, ncols / subCols>
    partition();

protected:
    T& at(uint16_t row, uint16_t col) { return d_[row * (ncols + skipCols_) + col]; }

    T at(uint16_t row, uint16_t col) const { return d_[row * (ncols + skipCols_) + col]; }

    T* d_{nullptr};
    uint16_t skipCols_{0};
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

    Matrix<T, nrows, ncols>& operator=(std::initializer_list<T> il)
    {
        ::mart::Matrix<T, nrows, ncols>::operator=(std::move(il));
        return *this;
    }

private:
    T data_[nrows * ncols]{};
};

}  // namespace alloc

template <class T, uint16_t nrows, uint16_t ncols>
Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator=(std::initializer_list<T> il)
{
    uint16_t row = 0;
    uint16_t col = 0;
    for (auto it = il.begin(); it != il.end(); ++it) {
        at(row, col) = *it;
        col++;
        if (col == ncols) {
            col = 0;
            row++;
        }
    }
    return *this;
}

template <class T, uint16_t nrows, uint16_t ncols>
alloc::Matrix<T, nrows, ncols> Matrix<T, nrows, ncols>::operator+(const Matrix<T, nrows, ncols>& rhs) const
{
    alloc::Matrix<T, nrows, ncols> result;
    for (uint16_t row = 0; row < nrows; ++row) {
        for (uint16_t col = 0; col < ncols; ++col) {
            result(row, col) = at(row, col) + rhs(row, col);
        }
    }
    return result;
}

template <class T, uint16_t nrows, uint16_t ncols>
Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator+=(const Matrix<T, nrows, ncols>& rhs)
{
    for (uint16_t row = 0; row < nrows; ++row) {
        for (uint16_t col = 0; col < ncols; ++col) {
            at(row, col) += rhs(row, col);
        }
    }
    return *this;
}

template <class T, uint16_t nrows, uint16_t ncols>
alloc::Matrix<T, nrows, ncols> Matrix<T, nrows, ncols>::operator*(T mul) const
{
    alloc::Matrix<T, nrows, ncols> result;
    for (uint16_t row = 0; row < nrows; ++row) {
        for (uint16_t col = 0; col < ncols; ++col) {
            result(row, col) = at(row, col) * mul;
        }
    }
    return result;
}

template <class T, uint16_t nrows, uint16_t ncols>
Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator*=(T mul) const
{
    for (uint16_t row = 0; row < nrows; ++row) {
        for (uint16_t col = 0; col < ncols; ++col) {
            at(row, col) *= mul;
        }
    }
    return *this;
}

template <class T, uint16_t nrows, uint16_t ncols>
alloc::Vector<T, nrows> Matrix<T, nrows, ncols>::operator*(const Vector<T, ncols>& vec) const
{
    alloc::Vector<T, nrows> result;
    for (uint16_t row = 0; row < nrows; ++row) {
        for (uint16_t col = 0; col < ncols; ++col) {
            result[row] += at(row, col) * vec[col];
        }
    }
    return result;
}

template <class T, uint16_t nrows, uint16_t ncols>
template <uint16_t size>
alloc::Matrix<T, nrows, size> Matrix<T, nrows, ncols>::operator*(const Matrix<T, ncols, size>& mat) const
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

template <class T, uint16_t nrows, uint16_t ncols>
alloc::Matrix<T, ncols, nrows> Matrix<T, nrows, ncols>::transpose() const
{
    alloc::Matrix<T, nrows, ncols> result;
    transpose(result);
    return result;
}

template <class T, uint16_t nrows, uint16_t ncols>
void Matrix<T, nrows, ncols>::transpose(Matrix<T, ncols, nrows>& tr) const
{
    for (uint16_t row = 0; row < nrows; ++row) {
        for (uint16_t col = 0; col < ncols; ++col) {
            tr(row, col) = at(col, row);
        }
    }
}

template <class T, uint16_t nrows, uint16_t ncols>
void Matrix<T, nrows, ncols>::luDecompose(Matrix<T, nrows, nrows>& L, Matrix<T, nrows, nrows>& U) const
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

template <class T, uint16_t nrows, uint16_t ncols>
alloc::Matrix<T, nrows, ncols> Matrix<T, nrows, ncols>::eye()
{
    alloc::Matrix<T, nrows, ncols> result;
    for (uint16_t i = 0; i < nrows; ++i) {
        result(i, i) = 1;
    }
    return result;
}

template <class T, uint16_t nrows, uint16_t ncols>
template <uint16_t subRows, uint16_t subCols>
Matrix<T, subRows, subCols>
Matrix<T, nrows, ncols>::submat(uint16_t fromRow, uint16_t fromCol)
{
    return Matrix<T, subRows, subCols>(&at(fromRow, fromCol), ncols + skipCols_ - subCols);
}

template <class T, uint16_t nrows, uint16_t ncols>
template <uint16_t subRows, uint16_t subCols>
alloc::Matrix<Matrix<T, subRows, subCols>, nrows / subRows, ncols / subCols>
Matrix<T, nrows, ncols>::partition()
{
    alloc::Matrix<Matrix<T, subRows, subCols>, nrows / subRows, ncols / subCols> P;
    for (uint16_t i = 0; i < nrows / subRows; ++i) {
        for (uint16_t j = 0; j < ncols / subCols; ++j) {
            P(i, j) = submat<subRows, subCols>(i * subRows, j * subCols);
        }
    }
    return P;
}

}

#endif /* MATRIX_H */
