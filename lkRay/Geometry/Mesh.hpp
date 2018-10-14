#pragma once

#include "Triangle.hpp"
#include "Primitive.hpp"

#include <vector>


namespace lkRay {
namespace Geometry {

class Mesh: public Primitive
{
    std::vector<Triangle> mTriangles;

public:
    Mesh(const lkCommon::Math::Vector4& pos, const std::vector<Triangle>& tris);
    ~Mesh() = default;

    bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) override;
};

} // namespace Geometry
} // namespace lkRay
