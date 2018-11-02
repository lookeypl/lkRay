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

lkCommon::Utils::PixelFloat4 Light::Sample(const Scene::RayCollision& collision) const
{
    // TODO move to PointLight
    lkCommon::Math::Vector4 lightRayDir(mPosition - collision.mCollisionPoint);
    float distToLight = lightRayDir.Length();
    lightRayDir = lightRayDir.Normalize();

    float lightCoeff = lightRayDir.Dot(collision.mCollisionNormal);
    if (lightCoeff < 0.0f)
        lightCoeff = 0.0f;

    return mColor * lightCoeff * (1.0f / (1.0f + mAttentuationFactor * distToLight * distToLight));
}

} // namespace Scene
} // namespace lkRay
