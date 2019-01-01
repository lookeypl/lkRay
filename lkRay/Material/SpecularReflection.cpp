#include "PCH.hpp"
#include "SpecularReflection.hpp"


namespace lkRay {
namespace Material {

lkCommon::Utils::PixelFloat4 SpecularReflection::F(const lkCommon::Math::Vector4& in, const lkCommon::Math::Vector4& normal, lkCommon::Math::Vector4& out)
{
    out = in - (normal * in.Dot(normal) * 2.0f);
    return lkCommon::Utils::PixelFloat4(1.0f);
}

} // namespace Material
} // namespace lkRay
