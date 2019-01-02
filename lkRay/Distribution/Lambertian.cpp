#include "PCH.hpp"
#include "Lambertian.hpp"


namespace lkRay {
namespace Distribution {

lkCommon::Utils::PixelFloat4 Lambertian::F(const lkCommon::Math::Vector4& in, const lkCommon::Math::Vector4& normal, lkCommon::Math::Vector4& out)
{
    LKCOMMON_UNUSED(in);
    LKCOMMON_UNUSED(normal);
    LKCOMMON_UNUSED(out);
    return mR;
}

} // namespace Distribution
} // namespace lkRay
