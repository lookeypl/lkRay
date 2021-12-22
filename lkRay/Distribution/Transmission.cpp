#include "PCH.hpp"
#include "Transmission.hpp"


namespace lkRay {
namespace Distribution {

lkCommon::Utils::PixelFloat4 Transmission::F(const Renderer::PathContext& context,
                                             const Renderer::RayCollision& collision,
                                             lkCommon::Math::Vector4& out)
{
    // i = incoming
    // n = normal
    // out = transmitted
    const lkCommon::Math::Vector4& i = context.ray.mDirection;
    const lkCommon::Math::Vector4& n = collision.mNormal;

    const float cosomega = -i.Dot(n);
    const float sin2omega = (mIOR * mIOR) * (1 - (cosomega * cosomega));
    out = i * mIOR + n * (mIOR * cosomega - sqrtf(1 - sin2omega));

    return lkCommon::Utils::PixelFloat4(mContribution);
}

} // namespace Distribution
} // namespace lkRay
