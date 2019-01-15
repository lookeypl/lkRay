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

void Plane::CalculateBBox()
{
    // Planes have infinite distance - assign +/-inf just for formality sake
    mBBox[AABBPoint::MIN] = lkCommon::Math::Vector4(-std::numeric_limits<float>::infinity());
    mBBox[AABBPoint::MAX] = lkCommon::Math::Vector4(std::numeric_limits<float>::infinity());

    mBBox[AABBPoint::MIN][3] = 1.0f;
    mBBox[AABBPoint::MAX][3] = 1.0f;
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

Types::Primitive Plane::GetType() const
{
    return Types::Primitive::PLANE;
}

bool Plane::ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials)
{
    // read only material - position depends on plane-specific normal and distance
    if (!ReadMaterialFromNode(value, materials))
    {
        LOGE("Failed to read object's material");
        return false;
    }

    // read normal and distance
    bool normalFound = false;
    bool distanceFound = false;

    for (auto& a: value.GetObject())
    {
        if (Constants::PLANE_ATTRIBUTE_NORMAL_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray() || (a.value.GetArray().Size() != 3))
            {
                LOGE("Invalid normal parameter for plane " << mName <<
                     ". Should be an array of 3 floats.");
                return false;
            }

            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                mNormal[colIndex] = c.GetFloat();
                colIndex++;
            }

            mNormal[3] = 0.0f;

            LOGD("     -> Plane's normal " << mNormal);
            normalFound = true;
        }
        else if (Constants::PLANE_ATTRIBUTE_DISTANCE_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsFloat())
            {
                LOGE("Invalid distance parameter for plane " << mName <<
                     ". Should be a float number.");
                return false;
            }

            mD = a.value.GetFloat();
            LOGD("     -> Plane's distance " << mD);
            distanceFound = true;
        }
    }

    if (!(distanceFound && normalFound))
    {
        LOGE("Not all parameters were found for plane " << mName);
        return false;
    }

    return true;
}

} // namespace Geometry
} // namespace lkRay
