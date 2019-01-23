#pragma once

#include "Light.hpp"


namespace lkRay {
namespace Light {

class DirLight: public Light
{
private:
    lkCommon::Math::Vector4 mDirection;

public:
    DirLight(const std::string& name);
    DirLight(const std::string& name, const lkCommon::Utils::PixelFloat4& color, const lkCommon::Math::Vector4& dir);
    ~DirLight() = default;

    lkCommon::Math::Vector4 GetToLightDir(const Renderer::RayCollision& collision) const override;
    lkCommon::Utils::PixelFloat4 Sample(const Renderer::RayCollision& collision) const override;
    bool ReadParametersFromNode(const rapidjson::Value& value) override;
};

} // namespace Light
} // namespace lkRay
