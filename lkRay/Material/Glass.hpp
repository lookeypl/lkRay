#pragma once

#include "Material.hpp"


namespace lkRay {
namespace Material {

class Glass : public Material
{
    float mDensity;

public:
    Glass(const std::string& name);
    Glass(const std::string& name, const float& density);
    ~Glass() = default;

    void SetDensity(const float density);
    void PopulateDistributionFunctions(Renderer::PathContext& context, Renderer::RayCollision& collision) override;
    bool ReadParametersFromNode(const rapidjson::Value& value) override;
};

} // namespace Material
} // namespace lkRay

