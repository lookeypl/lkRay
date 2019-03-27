#pragma once

#include "Material.hpp"
#include <lkCommon/Utils/Image.hpp>


namespace lkRay {
namespace Material {

class Matte: public Material
{
    lkCommon::Utils::Image<lkCommon::Utils::PixelFloat4> mTexture;

public:
    Matte(const std::string& name);
    Matte(const std::string& name, const lkCommon::Utils::PixelFloat4& color);
    ~Matte() = default;

    bool LoadTexture(const std::string& path);
    void SetColor(const lkCommon::Utils::PixelFloat4& color);
    void PopulateDistributionFunctions(Renderer::RayCollision& collision) override;
    bool ReadParametersFromNode(const rapidjson::Value& value) override;
};

} // namespace Material
} // namespace lkRay
