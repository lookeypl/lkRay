#include "PCH.hpp"
#include "Emissive.hpp"


namespace lkRay {
namespace Distribution {

lkCommon::Utils::PixelFloat4 Emissive::F(const Renderer::PathContext& context,
                                         const Renderer::RayCollision& collision,
                                         lkCommon::Math::Vector4& out)
{
    return mI;
}

} // namespace Distribution
} // namespace lkRay
