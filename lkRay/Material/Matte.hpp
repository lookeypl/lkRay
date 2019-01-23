#pragma once

#include "Material.hpp"


namespace lkRay {
namespace Material {

class Matte: public Material
{
    lkCommon::Utils::PixelFloat4 mColor;

public:
    Matte(const std::string& name);
    Matte(const std::string& name, const lkCommon::Utils::PixelFloat4& color);
    ~Matte() = default;

    void PopulateDistributionFunctions(Renderer::RayCollision& collision) override;
    bool ReadParametersFromNode(const rapidjson::Value& value) override;

    LKCOMMON_INLINE void SetColor(const lkCommon::Utils::PixelFloat4& color)
    {
        mColor = color;
    }

    LKCOMMON_INLINE const lkCommon::Utils::PixelFloat4& GetColor() const
    {
        return mColor;
    }
};

} // namespace Material
} // namespace lkRay
