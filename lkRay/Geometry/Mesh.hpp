#pragma once

#include "Triangle.hpp"
#include "Primitive.hpp"

#include "Scene/Containers.hpp"
#include "Scene/BVH.hpp"


namespace lkRay {
namespace Geometry {

class Mesh: public Primitive
{
    std::vector<lkCommon::Math::Vector4> mPoints;
    Scene::BVH<Triangle> mMeshBVH;

public:
    Mesh(const std::string& name);
    Mesh(const std::string& name, const lkCommon::Math::Vector4& pos, const std::vector<lkCommon::Math::Vector4>& points,
         const std::vector<Triangle>& indices);
    ~Mesh() = default;

    bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) const override;
    void CalculateBBox() override;
    bool ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials) override;
    Types::Primitive GetType() const override;
};

} // namespace Geometry
} // namespace lkRay
