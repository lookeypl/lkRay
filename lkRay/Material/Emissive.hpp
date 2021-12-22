#pragma once

#include "Material.hpp"


namespace lkRay {
namespace Material {

class Emissive: public Material
{
    lkCommon::Utils::PixelFloat4 mIntensity;

public:
    Emissive(const std::string& name);
    Emissive(const std::string& name, const lkCommon::Utils::PixelFloat4& intensity);
    ~Emissive() = default;

    void SetIntensity(const lkCommon::Utils::PixelFloat4& intensity);
    void PopulateDistributionFunctions(Renderer::PathContext& context, Renderer::RayCollision& collision) override;
    bool ReadParametersFromNode(const rapidjson::Value& value) override;
};

} // namespace Material
} // namespace lkRay

