#include "PCH.hpp"
#include "Light.hpp"


namespace lkRay {
namespace Scene {

Light::Light(const lkCommon::Math::Vector4& pos, const lkCommon::Utils::PixelFloat4& color, float attenuation)
    : mPosition(pos)
    , mColor(color)
    , mAttentuationFactor(attenuation)
{
}

} // namespace Scene
} // namespace lkRay
