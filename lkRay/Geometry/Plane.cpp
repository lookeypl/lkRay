#include "PCH.hpp"
#include "Plane.hpp"

#include "Constants.hpp"


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
    float normalDotDir = mNormal.Dot(ray.mDirection);
    if (normalDotDir > 0)
        return false;

    distance = -(mNormal.Dot(ray.mOrigin) - mD) / normalDotDir;
    if (distance < 0)
        return false;

    normal = mNormal;
    return true;
}

bool Plane::ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials)
{
    // read only material - position depends on plane-specific normal and distance
    if (!ReadMaterial(value, materials))
    {
        LOGE("Failed to read object's material");
        return false;
    }

    // read normal and distance
    for (auto& a: value.GetObject())
    {
        if (Constants::PLANE_ATTRIBUTE_NORMAL_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                mNormal[colIndex] = c.GetFloat();
                colIndex++;
            }

            LOGD("     -> Plane's normal " << mNormal);
        }
        else if (Constants::PLANE_ATTRIBUTE_DISTANCE_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            mD = a.value.GetFloat();
            LOGD("     -> Plane's distance " << mD);
            break;
        }
    }

    return true;
}

} // namespace Geometry
} // namespace lkRay
