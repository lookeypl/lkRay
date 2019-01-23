#pragma once

#include "Material.hpp"


namespace lkRay {
namespace Material {

class Mirror: public Material
{
    lkCommon::Utils::PixelFloat4 mColor;

public:
    Mirror(const std::string& name);
    ~Mirror() = default;

    void PopulateDistributionFunctions(Renderer::RayCollision& collision) override;
    bool ReadParametersFromNode(const rapidjson::Value& value) override;
};

} // namespace Material
} // namespace lkRay
