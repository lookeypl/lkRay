#include "PCH.hpp"
#include "LambertianDistribution.hpp"


namespace lkRay {
namespace Material {

lkCommon::Utils::PixelFloat4 LambertianDistribution::F(const lkCommon::Math::Vector4& in, const lkCommon::Math::Vector4& normal, lkCommon::Math::Vector4& out)
{
    LKCOMMON_UNUSED(in);
    LKCOMMON_UNUSED(normal);
    LKCOMMON_UNUSED(out);
    return mR;
}

} // namespace Material
} // namespace lkRay
