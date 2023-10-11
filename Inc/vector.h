#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cstdint>
#include <initializer_list>

namespace mart
{

namespace alloc
{
template <class T, uint16_t size>
class Vector;
}

template <class T, uint16_t size>
class Vector
{
public:
    using Type                     = T;
    static constexpr uint16_t Size = size;

    explicit Vector(T* data);

    T& operator[](uint16_t i);

    T operator[](uint16_t i) const;

    alloc::Vector<T, size> operator+(const Vector<T, size>& rhs) const;

    Vector<T, size>& operator+=(const Vector<T, size>& rhs);

    alloc::Vector<T, size> operator*(T multiplier) const;

    Vector<T, size>& operator*=(T multiplier);

    template <uint16_t subSize>
    Vector<T, subSize> subvec(uint16_t from) const;

private:
    T* d_;
};

namespace alloc
{

template <class T, uint16_t size>
class Vector : public ::mart::Vector<T, size>
{
public:
    Vector() : ::mart::Vector<T, size>(data_) {}

    explicit Vector(const T* data) : Vector()
    {
        std::copy(data, data + size, data_);
    }

    Vector(const Vector<T, size>& other) : Vector(other.data_) {}

    Vector(std::initializer_list<T> il) : Vector()
    {
        std::copy(il.begin(), il.end(), data_);
    }

private:
    T data_[size]{};
};

}  // namespace alloc

template <typename T, uint16_t size>
Vector<T, size>::Vector(T* data) : d_(data)
{
}

template <typename T, uint16_t size>
T& Vector<T, size>::operator[](uint16_t i)
{
    return d_[i];
}

template <typename T, uint16_t size>
T Vector<T, size>::operator[](uint16_t i) const
{
    return d_[i];
}

template <typename T, uint16_t size>
alloc::Vector<T, size> Vector<T, size>::operator+(
    const Vector<T, size>& rhs) const
{
    alloc::Vector<T, size> result;
    for (uint16_t i = 0; i < size; ++i) {
        result[i] = d_[i] + rhs[i];
    }
    return result;
}

template <typename T, uint16_t size>
Vector<T, size>& Vector<T, size>::operator+=(const Vector<T, size>& rhs)
{
    for (uint16_t i = 0; i < size; ++i) {
        d_[i] += rhs[i];
    }
    return *this;
}

template <typename T, uint16_t size>
alloc::Vector<T, size> Vector<T, size>::operator*(T multiplier) const
{
    alloc::Vector<T, size> result;
    for (uint16_t i = 0; i < size; ++i) {
        result[i] = d_[i] * multiplier;
    }
    return result;
}

template <typename T, uint16_t size>
Vector<T, size>& Vector<T, size>::operator*=(T multiplier)
{
    for (uint16_t i = 0; i < size; ++i) {
        d_[i] *= multiplier;
    }
    return *this;
}

template <typename T, uint16_t size>
template <uint16_t subSize>
Vector<T, subSize> Vector<T, size>::subvec(uint16_t from) const
{
    return Vector<T, subSize>(d_ + from);
}

}  // namespace mart

#endif /* VECTOR_H */
