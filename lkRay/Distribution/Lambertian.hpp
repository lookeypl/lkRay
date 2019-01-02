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
        : Function(FunctionType::DIFFUSE | FunctionType::REFLECTION)
        , mR(r)
    { }

    lkCommon::Utils::PixelFloat4 F(const lkCommon::Math::Vector4& in, const lkCommon::Math::Vector4& normal, lkCommon::Math::Vector4& out) override;
};

} // namespace Distribution
} // namespace lkRay
