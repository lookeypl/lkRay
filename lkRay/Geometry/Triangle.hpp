#pragma once

#include <lkCommon/Math/Vector4.hpp>

#include "Ray.hpp"

#include <memory>


namespace lkRay {
namespace Geometry {

struct UV
{
    float u;
    float v;

    UV()
        : u(0)
        , v(0)
    {
    }

    UV(float U, float V)
        : u(U)
        , v(V)
    {
    }
};

// Triangle does not inherit Primitive, as it's a building block for Model
class Triangle
{
public:
    using Ptr = std::shared_ptr<Triangle>;

private:
    uint32_t mPoints[3];

public:
    Triangle();
    Triangle(uint32_t a, uint32_t b, uint32_t c);
    ~Triangle() = default;

    bool TestCollision(const lkCommon::Math::Vector4& pos, const std::vector<lkCommon::Math::Vector4>& points,
                       const Ray& ray, float& distance, lkCommon::Math::Vector4& normal);

    LKCOMMON_INLINE uint32_t& operator[](const size_t i)
    {
        LKCOMMON_ASSERT(i < 3, "Invalid index - triangles only have 3 indices");
        return mPoints[i];
    }
};

} // namespace Geometry
} // namespace lkRay
