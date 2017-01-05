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
   value.m = _mm_load_ps(v);
}

Vector::Vector(__m128 v)
{
    value.m = v;
}

Vector::Vector(const Vector& other)
{
    value.m = _mm_load_ps(other.value.f);
}

Vector::Vector(Vector&& other)
{
    value.m = _mm_load_ps(other.value.f);
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
    return Vector(*this) /= other;
}

const Vector Vector::operator/(float value) const
{
    return Vector(*this) /= value;
}

// Power
Vector& Vector::operator^(float value)
{
    __m128 exp = _mm_set_ps1(value);
    
    return *this;
}

// Products
float Vector::Dot(const Vector& other)
{
    __m128 result = _mm_dp_ps(value.m, other.value.m, 0xF1);
    return result.m128_f32[0];
}

Vector& Vector::Cross(const Vector& other)
{
    __m128 test = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
    __m128 test2 = _mm_shuffle_ps(test, test, _MM_SHUFFLE(3, 1, 0, 2));

    __m128 shufa1 = _mm_shuffle_ps(value.m, value.m, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 shufb1 = _mm_shuffle_ps(other.value.m, other.value.m, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 shufa2 = _mm_shuffle_ps(value.m, value.m, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 shufb2 = _mm_shuffle_ps(other.value.m, other.value.m, _MM_SHUFFLE(3, 0, 2, 1));

    __m128 mul1 = _mm_mul_ps(shufa1, shufb1);
    __m128 mul2 = _mm_mul_ps(shufa2, shufb2);
    value.m = _mm_sub_ps(mul1, mul2);
    return *this;
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
