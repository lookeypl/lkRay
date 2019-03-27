#pragma once

#include "Triangle.hpp"
#include "Primitive.hpp"

#include "Scene/Containers.hpp"
#include "Scene/BVH.hpp"

#include "Vertex.hpp"


namespace lkRay {
namespace Geometry {

class Mesh: public Primitive
{
    Vertices mPoints;
    Scene::BVH<Triangle> mMeshBVH;

public:
    Mesh(const std::string& name);
    Mesh(const std::string& name, const lkCommon::Math::Vector4& pos, const Vertices& points,
         const std::vector<Triangle>& indices);
    ~Mesh() = default;

    bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal, UV& uv) const override;
    void CalculateBBox() override;
    bool LoadFromFile(const std::string& objFilePath);
    bool ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials) override;
    Types::Primitive GetType() const override;
};

} // namespace Geometry
} // namespace lkRay
