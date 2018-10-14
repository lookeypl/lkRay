#pragma once

#include <lkCommon/Math/Vector4.hpp>

#include "Ray.hpp"

#include <memory>


namespace lkRay {
namespace Geometry {

// Triangle does not inherit Primitive, as it's a building block for Model
class Triangle
{
public:
    using Ptr = std::shared_ptr<Triangle>;

private:
    lkCommon::Math::Vector4 mPoints[3];

public:
    Triangle(const lkCommon::Math::Vector4& a, const lkCommon::Math::Vector4& b, const lkCommon::Math::Vector4& c);
    ~Triangle() = default;

    bool TestCollision(const lkCommon::Math::Vector4& pos, const Ray& ray, float& distance, lkCommon::Math::Vector4& normal);
};

} // namespace Geometry
} // namespace lkRay
