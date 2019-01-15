#pragma once

#include "Primitive.hpp"


namespace lkRay {
namespace Geometry {

class Plane: public Primitive
{
public:
    using Ptr = std::shared_ptr<Plane>;

private:
    lkCommon::Math::Vector4 mNormal;
    float mD;


public:
    Plane(const std::string& name);
    Plane(const std::string& name, const lkCommon::Math::Vector4& normal, float distance);
    ~Plane() = default;

    bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) override;
    void CalculateBBox() override;
    bool ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials) override;
    Types::Primitive GetType() const override;
};

} // namespace Geometry
} // namespace lkRay
