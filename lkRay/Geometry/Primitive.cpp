#include "PCH.hpp"
#include "Primitive.hpp"
#include "Constants.hpp"


namespace lkRay {
namespace Geometry {

Primitive::Primitive(const std::string& name)
    : mName(name)
    , mPosition(0.0f, 0.0f, 0.0f, 1.0f)
    , mMaterial(nullptr)
{
}

Primitive::Primitive(const std::string& name, const lkCommon::Math::Vector4& position)
    : mName(name)
    , mPosition(position)
    , mMaterial(nullptr)
{
}

bool Primitive::ReadPosition(const rapidjson::Value& value)
{
    for (auto& a: value.GetObject())
    {
        if (Constants::OBJECT_ATTRIBUTE_POSITION_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            uint32_t colIndex = 0;
            for (auto& c: a.value.GetArray())
            {
                mPosition[colIndex] = c.GetFloat();
                colIndex++;
            }

            LOGD("     -> Object position " << mPosition);
        }
    }

    return true;
}

bool Primitive::ReadMaterial(const rapidjson::Value& value, const Scene::Containers::Material& materials)
{
    std::string material;

    for (auto& a: value.GetObject())
    {
        if (Constants::OBJECT_ATTRIBUTE_MATERIAL_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            material = a.value.GetString();
            LOGD("     -> Object material " << material);
        }
    }

    if (material.empty())
    {
        LOGW("Object " << mName << " has no material assigned - using default");
        auto matIt = materials.find(Constants::LKRAY_DEFAULT_MATERIAL_NAME);
        if (matIt == materials.end())
        {
            LOGE("Couldn't find default material. This should not happen!");
            return false;
        }

        mMaterial = matIt->second.get();
        return true;
    }

    auto matIt = materials.find(material);
    if (matIt == materials.end())
    {
        LOGW("Object material " << material << " not found. Using default.");
        matIt = materials.find(Constants::LKRAY_DEFAULT_MATERIAL_NAME);
        if (matIt == materials.end())
        {
            LOGE("Couldn't find default material. This should not happen!");
            return false;
        }
    }

    mMaterial = matIt->second.get();
    return true;
}

bool Primitive::ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials)
{
    if (!ReadPosition(value))
    {
        LOGE("Failed to read object's position");
        return false;
    }

    if (!ReadMaterial(value, materials))
    {
        LOGE("Failed to read object's material");
        return false;
    }

    return true;
}

} // namespace Geometry
} // namespace lkRay
