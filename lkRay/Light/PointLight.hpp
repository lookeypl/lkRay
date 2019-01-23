#pragma once

#include "Light.hpp"


namespace lkRay {
namespace Light {

class PointLight: public Light
{
private:
    float mAttentuationFactor;

public:
    PointLight(const std::string& name);
    PointLight(const std::string& name, const lkCommon::Math::Vector4& pos, const lkCommon::Utils::PixelFloat4& color, float attenuation);
    ~PointLight() = default;

    lkCommon::Math::Vector4 GetToLightDir(const Renderer::RayCollision& collision) const override;
    lkCommon::Utils::PixelFloat4 Sample(const Renderer::RayCollision& collision) const override;
    bool ReadParametersFromNode(const rapidjson::Value& value) override;
};

} // namespace Light
} // namespace lkRay
