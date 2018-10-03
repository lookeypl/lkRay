#pragma once

#include "Primitive.hpp"


namespace lkRay {
namespace Geometry {

// Triangle does not inherit Primitive, as it's a building block for Model
class Plane: public Primitive
{
public:
    using Ptr = std::shared_ptr<Plane>;

private:
    lkCommon::Math::Vector4 mNormal;
    float mD;

public:
    Plane(const lkCommon::Math::Vector4& normal, float distance);
    ~Plane() = default;

    bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) override;
};

} // namespace Geometry
} // namespace lkRay
