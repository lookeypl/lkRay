#pragma once

#include "Primitive.hpp"


namespace lkRay {
namespace Geometry {

class Sphere: public Primitive
{
public:
    using Ptr = std::shared_ptr<Sphere>;

private:
    float mRadius;
    float mRadiusSquare;


public:
    Sphere(const std::string& name);
    Sphere(const std::string& name, const lkCommon::Math::Vector4& origin, float r);
    ~Sphere() = default;

    bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal, UV& uv) const override;
    void CalculateBBox() override;
    bool ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials) override;
    Types::Primitive GetType() const override;

    LKCOMMON_INLINE void SetRadius(float radius)
    {
        mRadius = radius;
        mRadiusSquare = mRadius * mRadius;
        CalculateBBox();
    }
};

} // namespace Geometry
} // namespace lkRay
