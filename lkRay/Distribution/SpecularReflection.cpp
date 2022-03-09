#include "PCH.hpp"
#include "SpecularReflection.hpp"


namespace lkRay {
namespace Distribution {

lkCommon::Utils::PixelFloat4 SpecularReflection::F(const Renderer::PathContext& context,
                                                   const Renderer::RayCollision& collision,
                                                   lkCommon::Math::Vector4& out)
{
    // a regular, perfect reflection equation
    const lkCommon::Math::Vector4& n = collision.mNormal;
    out = collision.mRay.mDirection - (n * collision.mRay.mDirection.Dot(n) * 2.0f);

    return lkCommon::Utils::PixelFloat4(mContribution);
}

} // namespace Distribution
} // namespace lkRay
