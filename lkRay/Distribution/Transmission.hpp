#pragma once

#include "Function.hpp"

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/Pixel.hpp>


namespace lkRay {
namespace Distribution {

class Transmission: public Function
{
    float mIOR;
    float mContribution;
    float _alignment[2]; // TODO MemoryAllocator has to be fixed for that

public:
    Transmission(float ior, float contribution)
        : Function(Types::Distribution::TRANSMISSION)
        , mIOR(ior)
        , mContribution(contribution)
    { }

    lkCommon::Utils::PixelFloat4 F(const Renderer::PathContext& context,
        const Renderer::RayCollision& collision,
        lkCommon::Math::Vector4& out) override;
};

} // namespace Distribution
} // namespace lkRay
