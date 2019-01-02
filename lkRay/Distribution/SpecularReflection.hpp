#pragma once

#include "Function.hpp"

#include <lkCommon/Utils/Pixel.hpp>


namespace lkRay {
namespace Distribution {

class SpecularReflection: public Function
{
public:
    SpecularReflection()
        : Function(FunctionType::SPECULAR | FunctionType::REFLECTION)
    { }

    lkCommon::Utils::PixelFloat4 F(const lkCommon::Math::Vector4& in, const lkCommon::Math::Vector4& normal, lkCommon::Math::Vector4& out) override;
};

} // namespace Distribution
} // namespace lkRay
