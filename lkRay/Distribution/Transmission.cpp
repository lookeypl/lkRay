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
    const lkCommon::Math::Vector4& i = collision.mRay.mDirection;
    const lkCommon::Math::Vector4& n = collision.mNormal;

    const float cosOmega = -i.Dot(n);
    const float sin2Omega = (mIOR * mIOR) * (1 - (cosOmega * cosOmega));
    out = i * mIOR + n * (mIOR * cosOmega - sqrtf(1 - sin2Omega));

    return lkCommon::Utils::PixelFloat4(mContribution);
}

} // namespace Distribution
} // namespace lkRay
