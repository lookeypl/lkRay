#pragma once

#include "Function.hpp"

#include <lkCommon/Utils/Pixel.hpp>


namespace lkRay {
namespace Distribution {

class SpecularReflection: public Function
{
public:
    SpecularReflection()
        : Function(Types::Distribution::SPECULAR | Types::Distribution::REFLECTION)
    { }

    lkCommon::Utils::PixelFloat4 F(const Renderer::PathContext& context,
                                   const Renderer::RayCollision& collision,
                                   lkCommon::Math::Vector4& out) override;
};

} // namespace Distribution
} // namespace lkRay
