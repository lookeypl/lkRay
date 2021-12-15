#pragma once

#include "Function.hpp"

#include <lkCommon/Utils/Pixel.hpp>


namespace lkRay {
namespace Distribution {

class Emissive: public Function
{
    lkCommon::Utils::PixelFloat4 mI;

public:
    Emissive(const lkCommon::Utils::PixelFloat4& i)
        : Function(Types::Distribution::EMISSIVE)
        , mI(i)
    { }

    lkCommon::Utils::PixelFloat4 F(const Renderer::PathContext& context,
                                   const Renderer::RayCollision& collision,
                                   lkCommon::Math::Vector4& out) override;
};

} // namespace Distribution
} // namespace lkRay
