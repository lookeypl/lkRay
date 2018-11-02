#pragma once

#include "Distribution.hpp"

#include <lkCommon/Utils/Pixel.hpp>


namespace lkRay {
namespace Material {

class LambertianDistribution: public Distribution
{
    lkCommon::Utils::PixelFloat4 mR;

public:
    LambertianDistribution(const lkCommon::Utils::PixelFloat4& r)
        : Distribution(DistributionType::DIFFUSE | DistributionType::REFLECTION)
        , mR(r)
    { }

    lkCommon::Utils::PixelFloat4 F(const lkCommon::Math::Vector4& in, const lkCommon::Math::Vector4& out) override;
};

} // namespace Material
} // namespace lkRay
