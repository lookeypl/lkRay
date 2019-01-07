#pragma once

#include "Light.hpp"


namespace lkRay {
namespace Scene {

class PointLight: public Light
{
private:
    float mAttentuationFactor;

public:
    PointLight(const std::string& name);
    PointLight(const std::string& name, const lkCommon::Math::Vector4& pos, const lkCommon::Utils::PixelFloat4& color, float attenuation);
    ~PointLight() = default;

    lkCommon::Utils::PixelFloat4 Sample(const RayCollision& collision) const override;
    bool ReadParametersFromNode(const rapidjson::Value& value) override;
};

} // namespace Scene
} // namespace lkRay
