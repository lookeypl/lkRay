#include "PCH.hpp"
#include "Lambertian.hpp"

#include <lkCommon/Math/Utilities.hpp>


namespace lkRay {
namespace Distribution {

lkCommon::Utils::PixelFloat4 Lambertian::F(const Renderer::PathContext& context, const Scene::RayCollision& collision,
                                           lkCommon::Math::Vector4& out)
{
    out = lkCommon::Math::Util::CosineSampleHemisphere(
        collision.mNormal,
        lkCommon::Math::Random::Xorshift(context.threadData.rngState),
        lkCommon::Math::Random::Xorshift(context.threadData.rngState)
    );

    return mR;
}

} // namespace Distribution
} // namespace lkRay
