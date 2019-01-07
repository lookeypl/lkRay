#include "PCH.hpp"
#include "Sphere.hpp"

#include <cmath>

#include "lkCommon/Math/Constants.hpp"

#include "Constants.hpp"


namespace lkRay {
namespace Geometry {

Sphere::Sphere(const std::string& name)
    : Primitive(name)
    , mRadiusSquare(1.0f)
{
}

Sphere::Sphere(const std::string& name, const lkCommon::Math::Vector4& origin, float r)
    : Primitive(name, origin)
    , mRadiusSquare(r*r)
{
}

bool Sphere::TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal)
{
    lkCommon::Math::Vector4 L = mPosition - ray.mOrigin;
    float midPoint = L.Dot(ray.mDirection);
    if (midPoint < LKCOMMON_EPSILON)
        return false; // sphere is behind us

    float distFromOriginSquare = L.Dot(L) - midPoint * midPoint;
    if (distFromOriginSquare < LKCOMMON_EPSILON)
        return false; // wut
    if (distFromOriginSquare > mRadiusSquare)
        return false; // missed sphere

    float midToIntersection = sqrtf(mRadiusSquare - distFromOriginSquare);
    float s0 = midPoint - midToIntersection;
    float s1 = midPoint + midToIntersection;

    distance = s0 < s1 ? s0 : s1;

    lkCommon::Math::Vector4 collisionPoint = ray.mOrigin + ray.mDirection * distance;
    normal = (collisionPoint - mPosition).Normalize();

    return true;
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
    float radius = 1.0f;
    for (auto& a: value.GetObject())
    {
        if (Constants::SPHERE_ATTRIBUTE_RADIUS_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            radius = a.value.GetFloat();
            LOGD("     -> Sphere radius " << radius);
            mRadiusSquare = radius * radius;
            break;
        }
    }

    return true;
}

} // namespace Geometry
} // namespace lkRay
