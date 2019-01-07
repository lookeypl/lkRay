#include "PCH.hpp"
#include "Plane.hpp"


namespace lkRay {
namespace Geometry {

Plane::Plane(const std::string& name)
    : Primitive(name)
    , mNormal()
    , mD(0.0f)
{
}

Plane::Plane(const std::string& name, const lkCommon::Math::Vector4& normal, float distance)
    : Primitive(name)
    , mNormal(normal)
    , mD(distance)
{
}

bool Plane::TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal)
{
    // Regular geometric approach checking if ray hit a plane:
    //  * Get dot product (angle between normal and ray dir)
    //  * Measure ray hit distance (<0 means we missed)
    float normalDotDir = mNormal.Dot(ray.GetDirection());
    if (normalDotDir > 0)
        return false;

    distance = -(mNormal.Dot(ray.GetOrigin()) - mD) / normalDotDir;
    if (distance < 0)
        return false;

    normal = mNormal;
    return true;
}

} // namespace Geometry
} // namespace lkRay
