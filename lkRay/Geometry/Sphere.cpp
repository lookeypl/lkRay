#include "PCH.hpp"
#include "Sphere.hpp"

#include <cmath>

#include "lkCommon/Math/Constants.hpp"

#include "Constants.hpp"


namespace lkRay {
namespace Geometry {

Sphere::Sphere(const std::string& name)
    : Primitive(name)
    , mRadius(1.0f)
    , mRadiusSquare(1.0f)
{
}

Sphere::Sphere(const std::string& name, const lkCommon::Math::Vector4& origin, float r)
    : Primitive(name, origin)
    , mRadius(r)
    , mRadiusSquare(mRadius * mRadius)
{
}

void Sphere::CalculateBBox()
{
    mBBox[AABBPoint::MIN] = lkCommon::Math::Vector4(-mRadius, -mRadius, -mRadius, 1.0f);
    mBBox[AABBPoint::MAX] = lkCommon::Math::Vector4(mRadius, mRadius, mRadius, 1.0f);
}

bool Sphere::TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal, UV& uv) const
{
    const lkCommon::Math::Vector4 L = mPosition - ray.mOrigin;
    const float midPoint = L.Dot(ray.mDirection);
    if (midPoint < LKCOMMON_EPSILON)
        return false; // sphere is behind us

    float distFromOriginSquare = L.Dot(L) - midPoint * midPoint;
    if (distFromOriginSquare < 0.0f)
        return false;
    if (distFromOriginSquare > mRadiusSquare)
        return false; // missed sphere

    const float radMinusDist = mRadiusSquare - distFromOriginSquare;
    if (radMinusDist < LKCOMMON_EPSILON)
        return false;

    const float midToIntersection = sqrtf(radMinusDist);
    float s0 = midPoint - midToIntersection;
    float s1 = midPoint + midToIntersection;

    if (s1 < s0)
        std::swap(s0, s1);

    distance = (s0 < LKCOMMON_EPSILON) ? s1 : s0;

    const lkCommon::Math::Vector4 collisionPoint = ray.mOrigin + ray.mDirection * distance;
    normal = (collisionPoint - mPosition).Normalize();
    uv.u = (1.0f + atan2f(normal[2], normal[0]) / LKCOMMON_PIF) * 0.5f;
    uv.v = acosf(normal[1]) / LKCOMMON_PIF;

    return true;
}

Types::Primitive Sphere::GetType() const
{
    return Types::Primitive::SPHERE;
}

bool Sphere::ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials)
{
    // read position and attached material
    if (!Primitive::ReadParametersFromNode(value, materials))
    {
        LOGE("Failed to read base parameters for primitive");
        return false;
    }

    // read sphere's radius
    for (auto& a: value.GetObject())
    {
        if (Constants::SPHERE_ATTRIBUTE_RADIUS_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsFloat())
            {
                LOGE("Invalid radius parameter for sphere " << mName <<
                     ". Should be a float number.");
                return false;
            }

            mRadius = a.value.GetFloat();
            LOGD("     -> Sphere radius " << mRadius);
            mRadiusSquare = mRadius * mRadius;
            return true;
        }
    }

    LOGE("Radius parameter not found for sphere " << mName);
    return false;
}

} // namespace Geometry
} // namespace lkRay
