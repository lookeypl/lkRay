#pragma once

#include "Material.hpp"


namespace lkRay {
namespace Material {

class Mirror: public Material
{
    lkCommon::Utils::PixelFloat4 mColor;

public:
    Mirror();
    ~Mirror() = default;

    void PopulateDistributionFunctions(Scene::RayCollision& collision) override;
};

} // namespace Material
} // namespace lkRay
