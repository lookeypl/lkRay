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
    Mesh(const std::string& name);
    Mesh(const std::string& name, const lkCommon::Math::Vector4& pos, const std::vector<lkCommon::Math::Vector4>& points,
         const std::vector<Triangle>& indices);
    ~Mesh() = default;

    bool TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) override;
    bool ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials) override;
};

} // namespace Geometry
} // namespace lkRay
