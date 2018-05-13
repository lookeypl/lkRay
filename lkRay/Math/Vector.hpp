#pragma once

#include <xmmintrin.h>


namespace lkRay {
namespace Math {

__declspec(align(16))
class Vector final
{
    union Vector4f
    {
        __m128 m;
        float f[4];
    } value;

public:
    Vector();
    Vector(const float x);
    Vector(const float x, const float y, const float z, const float w);
    Vector(const float* v);
    Vector(const __m128 v);
    Vector(const Vector& other);
    Vector(Vector&& other);
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other);
    ~Vector();

    const float* Data() const;
    float Length() const;
    void Normalize();

    // Access operator
    float operator[](int index) const;

    // Addition
    Vector& operator+=(const Vector& other);
    Vector& operator+=(const float value);
    const Vector operator+(const Vector& other) const;
    const Vector operator+(const float value) const;

    // Subtraction
    Vector& operator-=(const Vector& other);
    Vector& operator-=(const float value);
    const Vector operator-(const Vector& other) const;
    const Vector operator-(const float value) const;

    // Multiplication
    Vector& operator*=(const Vector& other);
    Vector& operator*=(const float value);
    const Vector operator*(const Vector& other) const;
    const Vector operator*(const float value) const;

    // Division
    Vector& operator/=(const Vector& other);
    Vector& operator/=(const float value);
    const Vector operator/(const Vector& other) const;
    const Vector operator/(const float value) const;

    // Products
    float Dot(const Vector& other);
    Vector& Cross(const Vector& other);

    // Comparison
    bool operator==(const Vector& other) const;
    bool operator<(const Vector& other) const;
    bool operator>(const Vector& other) const;
    bool operator<=(const Vector& other) const;
    bool operator>=(const Vector& other) const;

    // Friendships
    friend std::ostream& operator<<(std::ostream& os, const Vector& v);
};

} // namespace Math
} // namespace lkRay
