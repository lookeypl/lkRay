#pragma once

#include "Function.hpp"

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/Pixel.hpp>


namespace lkRay {
namespace Distribution {

class Transmission: public Function
{
    float mIOR;

public:
    Transmission(float ior)
        : Function(Types::Distribution::TRANSMISSION)
        , mIOR(ior)
    { }

    lkCommon::Utils::PixelFloat4 F(const Renderer::PathContext& context,
        const Renderer::RayCollision& collision,
        lkCommon::Math::Vector4& out) override;
};

} // namespace Distribution
} // namespace lkRay
