#pragma once

#include "Triangle.hpp"
#include "Primitive.hpp"

#include <vector>


namespace lkRay {
namespace Geometry {

class Mesh: public Primitive
{
    std::vector<lkCommon::Math::Vector4> mPoints;
    std::vector<Triangle> mTriangleIndices;

public:
    Mesh(const lkCommon::Math::Vector4& pos, const std::vector<lkCommon::Math::Vector4>& points,
         const std::vector<Triangle>& indices);
    ~Mesh() = default;

    bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) override;
};

} // namespace Geometry
} // namespace lkRay
