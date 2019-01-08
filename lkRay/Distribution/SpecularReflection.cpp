#include "PCH.hpp"
#include "SpecularReflection.hpp"


namespace lkRay {
namespace Distribution {

lkCommon::Utils::PixelFloat4 SpecularReflection::F(const Renderer::PathContext& context, const Scene::RayCollision& collision,
                                                   lkCommon::Math::Vector4& out)
{
    // a regular, perfect reflection equation
    out = context.ray.mDirection - (collision.mNormal * context.ray.mDirection.Dot(collision.mNormal) * 2.0f);

    return lkCommon::Utils::PixelFloat4(1.0f);
}

} // namespace Distribution
} // namespace lkRay
