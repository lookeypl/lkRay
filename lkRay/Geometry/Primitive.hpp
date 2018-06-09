#pragma once

#include <lkCommon/Math/Vector4.hpp>

#include "Ray.hpp"


namespace lkRay {
namespace Geometry {

class Primitive
{
public:
    using Ptr = std::shared_ptr<Primitive>;

    enum class Type: unsigned char
    {
        Unknown = 0,
        Sphere,
    };

protected:
    lkCommon::Math::Vector4 mPosition;

public:
    Primitive();
    Primitive(const lkCommon::Math::Vector4& position);
    ~Primitive() = default;

    // returns closest positive distance to ray origin as argument and surface normal at collision point
    // false if there's no collision (ray missed the sphere), then both distance and normal are unmodified
    virtual bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) = 0;
};

} // namespace Geometry
} // namespace lkRay
