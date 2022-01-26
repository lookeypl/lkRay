#include "PCH.hpp"
#include "SpecularReflection.hpp"


namespace lkRay {
namespace Distribution {

lkCommon::Utils::PixelFloat4 SpecularReflection::F(const Renderer::PathContext& context,
                                                   const Renderer::RayCollision& collision,
                                                   lkCommon::Math::Vector4& out)
{
    // a regular, perfect reflection equation
    const lkCommon::Math::Vector4 n = collision.mNormal * (collision.mHitInside ? -1.0f : 1.0f);
    out = context.ray.mDirection - (n * context.ray.mDirection.Dot(n) * 2.0f);

    return lkCommon::Utils::PixelFloat4(mContribution);
}

} // namespace Distribution
} // namespace lkRay
