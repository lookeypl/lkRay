#pragma once

#include "Light.hpp"


namespace lkRay {
namespace Scene {

class PointLight: public Light
{
private:
    float mAttentuationFactor;

public:
    PointLight(const lkCommon::Math::Vector4& pos, const lkCommon::Utils::PixelFloat4& color, float attenuation);
    ~PointLight() = default;

    lkCommon::Utils::PixelFloat4 Sample(const RayCollision& collision) const override;

    LightType GetType() const override
    {
        return LightType::POINT;
    }
};

} // namespace Scene
} // namespace lkRay
