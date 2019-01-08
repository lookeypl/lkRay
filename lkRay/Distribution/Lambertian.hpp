#pragma once

#include "Function.hpp"

#include <lkCommon/Utils/Pixel.hpp>


namespace lkRay {
namespace Distribution {

class Lambertian: public Function
{
    lkCommon::Utils::PixelFloat4 mR;

public:
    Lambertian(const lkCommon::Utils::PixelFloat4& r)
        : Function(Types::Distribution::DIFFUSE | Types::Distribution::REFLECTION)
        , mR(r)
    { }

    lkCommon::Utils::PixelFloat4 F(const Renderer::PathContext& context, const Scene::RayCollision& collision,
                                   lkCommon::Math::Vector4& out) override;
};

} // namespace Distribution
} // namespace lkRay
