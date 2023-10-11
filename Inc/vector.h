#ifndef VECTOR_H
#define VECTOR_H

#include <initializer_list>
#include <algorithm>
#include <cstdint>

namespace mart
{

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

    Vector<T, size> operator*(T multiplier) const
    {
        Vector<T, size> result;
        for (uint16_t i = 0; i < size; ++i) {
            result[i] = data_[i] * multiplier;
        }
        return result;
    }

    Vector<T, size>& operator*=(T multiplier) const
    {
        for (uint16_t i = 0; i < size; ++i) {
            data_[i] *= multiplier;
        }
        return *this;
    }

    template <uint16_t subSize>
    Vector<T, subSize> subvec(uint16_t from) const
    {
        Vector<T, subSize> v;
        for (uint16_t i = 0; i < subSize; ++i) {
            v[i] = data_[from + i];
        }
        return v;
    }

private:
    T data_[size]{};
};

}

#endif /* VECTOR_H */
