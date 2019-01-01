#pragma once

#include "Distribution.hpp"

#include <lkCommon/Utils/Pixel.hpp>


namespace lkRay {
namespace Material {

class SpecularReflection: public Distribution
{
public:
    SpecularReflection()
        : Distribution(DistributionType::SPECULAR | DistributionType::REFLECTION)
    { }

    lkCommon::Utils::PixelFloat4 F(const lkCommon::Math::Vector4& in, const lkCommon::Math::Vector4& normal, lkCommon::Math::Vector4& out) override;
};

} // namespace Material
} // namespace lkRay
