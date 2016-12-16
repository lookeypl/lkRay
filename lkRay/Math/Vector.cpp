#include "../PCH.hpp"
#include "Vector.hpp"

namespace lkRay {
namespace Math {

Vector::Vector()
{
    value.m = _mm_setzero_ps();
}

Vector::Vector(const float x)
{
    value.m = _mm_set_ps1(x);
}

Vector::Vector(const float x, const float y, const float z, const float w)
{
    value.m = _mm_set_ps(x, y, z, w);
}

Vector::Vector(const float* v)
{
    _mm_load_ps(v);
}

Vector::Vector(__m128 v)
{
    value.m = v;
}

Vector::Vector(const Vector& other)
{

}

Vector::Vector(Vector&& other)
{
}

Vector& Vector::operator=(const Vector& other)
{
    return *this;
}

Vector& Vector::operator=(Vector&& other)
{
    return *this;
}

Vector::~Vector()
{
}

const float* Vector::Data() const
{
    return value.f;
}

float Vector::Length() const
{
    return 0.0f;
}

void Vector::Normalize()
{
}

// Access operator
float Vector::operator[](int index) const
{
    return value.f[index];
}

// Addition
Vector& Vector::operator+=(const Vector& other)
{
    value.m = _mm_add_ps(value.m, other.value.m);
    return *this;
}

Vector& Vector::operator+=(float value)
{
    __m128 v = _mm_set_ps1(value);
    this->value.m = _mm_add_ps(this->value.m, v);
    return *this;
}

const Vector Vector::operator+(const Vector& other) const
{
    return Vector(*this) += other;
}

const Vector Vector::operator+(float value) const
{
    return Vector(*this) += value;
}

// Subtraction
Vector& Vector::operator-=(const Vector& other)
{
    value.m = _mm_sub_ps(value.m, other.value.m);
    return *this;
}

Vector& Vector::operator-=(float value)
{
    __m128 v = _mm_set_ps1(value);
    this->value.m = _mm_sub_ps(this->value.m, v);
    return *this;
}

const Vector Vector::operator-(const Vector& other) const
{
    return Vector(*this) -= other;
}

const Vector Vector::operator-(float value) const
{
    return Vector(*this) -= value;
}

// Multiplication
Vector& Vector::operator*=(const Vector& other)
{
    value.m = _mm_mul_ps(value.m, other.value.m);
    return *this;
}

Vector& Vector::operator*=(float value)
{
    __m128 v = _mm_set_ps1(value);
    this->value.m = _mm_mul_ps(this->value.m, v);
    return *this;
}

const Vector Vector::operator*(const Vector& other) const
{
    return Vector(*this) *= other;
}

const Vector Vector::operator*(float value) const
{
    return Vector(*this) *= value;
}

// Division
Vector& Vector::operator/=(const Vector& other)
{
    value.m = _mm_div_ps(value.m, other.value.m);
    return *this;
}

Vector& Vector::operator/=(float value)
{
    __m128 v = _mm_set_ps1(value);
    this->value.m = _mm_div_ps(this->value.m, v);
    return *this;
}

const Vector Vector::operator/(const Vector& other) const
{
    return Vector(*this) *= other;
}

const Vector Vector::operator/(float value) const
{
    return Vector(*this) *= value;
}

// Power
Vector& Vector::operator^(float value)
{
    return *this;
}

// Products
float Vector::Dot(const Vector& other)
{
    return 0.0f;
}

Vector Vector::Cross(const Vector& other) const
{
    return Vector();
}

// Comparison
bool Vector::operator==(const Vector& other) const
{
    __m128 m = _mm_cmpeq_ps(value.m, other.value.m);
    return m.m128_u32[0] && m.m128_u32[1] && m.m128_u32[2] && m.m128_u32[3];
}

bool Vector::operator<(const Vector& other) const
{
    __m128 m = _mm_cmplt_ps(value.m, other.value.m);
    return m.m128_u32[0] && m.m128_u32[1] && m.m128_u32[2] && m.m128_u32[3];
}

bool Vector::operator>(const Vector& other) const
{
    __m128 m = _mm_cmpgt_ps(value.m, other.value.m);
    return m.m128_u32[0] && m.m128_u32[1] && m.m128_u32[2] && m.m128_u32[3];
}

bool Vector::operator<=(const Vector& other) const
{
    __m128 m = _mm_cmple_ps(value.m, other.value.m);
    return m.m128_u32[0] && m.m128_u32[1] && m.m128_u32[2] && m.m128_u32[3];
}

bool Vector::operator>=(const Vector& other) const
{
    __m128 m = _mm_cmpge_ps(value.m, other.value.m);
    return m.m128_u32[0] && m.m128_u32[1] && m.m128_u32[2] && m.m128_u32[3];
}

// Friendships
std::ostream& operator<<(std::ostream& os, const Vector& v)
{
    os << "[" << v.value.f[0] << ", " << v.value.f[1] << "," << v.value.f[2] << "," << v.value.f[3] << "]";
    return os;
}

} // namespace Math
} // namespace lkRay
