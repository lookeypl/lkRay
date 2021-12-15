#include "PCH.hpp"
#include "Scene.hpp"

#include "Constants.hpp"

#include "Material/Material.hpp"
#include "Material/Matte.hpp"
#include "Material/Mirror.hpp"
#include "Material/Emissive.hpp"

#include "Geometry/Sphere.hpp"
#include "Geometry/Plane.hpp"
#include "Geometry/Mesh.hpp"

#include <limits>
#include <fstream>
#include <string>
#include <streambuf>

#include <tiny_obj_loader.h>


namespace {

template <typename Base, typename Derived>
LKCOMMON_INLINE std::shared_ptr<Base> CreatePtr(const std::string& name)
{
    return std::dynamic_pointer_cast<Base>(std::make_shared<Derived>(name));
}

} // namespace


namespace lkRay {
namespace Scene {

Scene::Scene()
{
}

Scene::~Scene()
{
}

bool Scene::LoadMaterials(const rapidjson::Value& node)
{
    if (!node.IsObject())
    {
        LOGE("Invalid object node");
        return false;
    }

    for (auto& o: node.GetObject())
    {
        LOGD(" -> Material " << o.name.GetString());

        // read material's type
        Types::Material type = Types::Material::UNKNOWN;
        for (auto& a: o.value.GetObject())
        {
            if (Constants::MATERIAL_ATTRIBUTE_TYPE_NODE_NAME.compare(a.name.GetString()) == 0)
            {
                LOGD("     -> Material type " << a.value.GetString());

                if (Constants::MATERIAL_MATTE_NODE_NAME.compare(a.value.GetString()) == 0)
                {
                    type = Types::Material::MATTE;
                }
                else if (Constants::MATERIAL_MIRROR_NODE_NAME.compare(a.value.GetString()) == 0)
                {
                    type = Types::Material::MIRROR;
                }
                else if (Constants::MATERIAL_EMISSIVE_NODE_NAME.compare(a.value.GetString()) == 0)
                {
                    type = Types::Material::EMISSIVE;
                }
                else
                {
                    LOGE("Unknown material type read from scene JSON: " << a.value.GetString());
                }
            }
        }

        // create material
        Containers::Ptr<Material::Material> pMat = CreateMaterial(o.name.GetString(), type);
        if (!pMat)
        {
            LOGE("Failed to create material");
            return false;
        }

        // ask material to read its own parameters
        if (!pMat->ReadParametersFromNode(o.value))
        {
            LOGE("Material failed to read its parameters");
            return false;
        }
    }

    return true;
}

bool Scene::LoadObjects(const rapidjson::Value& node)
{
    if (!node.IsObject())
    {
        LOGE("Invalid object node");
        return false;
    }

    for (auto& o: node.GetObject())
    {
        LOGD(" -> Object " << o.name.GetString());

        // read object's type
        Types::Primitive type = Types::Primitive::UNKNOWN;
        for (auto& a: o.value.GetObject())
        {
            if (Constants::OBJECT_ATTRIBUTE_TYPE_NODE_NAME.compare(a.name.GetString()) == 0)
            {
                LOGD("     -> Object type " << a.value.GetString());

                if (Constants::OBJECT_SPHERE_NODE_NAME.compare(a.value.GetString()) == 0)
                {
                    type = Types::Primitive::SPHERE;
                }
                else if (Constants::OBJECT_PLANE_NODE_NAME.compare(a.value.GetString()) == 0)
                {
                    type = Types::Primitive::PLANE;
                }
                else if (Constants::OBJECT_MESH_NODE_NAME.compare(a.value.GetString()) == 0)
                {
                    type = Types::Primitive::MESH;
                }
                else
                {
                    LOGE("Unknown object type read from scene JSON: " << a.value.GetString());
                }
            }
        }

        // create object
        Containers::Ptr<Geometry::Primitive> pPrim = CreatePrimitive(o.name.GetString(), type);
        if (!pPrim)
        {
            LOGE("Failed to create object");
            return false;
        }

        // ask object to read its own parameters
        if (!pPrim->ReadParametersFromNode(o.value, mMaterials))
        {
            LOGE("Object failed to read its parameters");
            return false;
        }

        pPrim->CalculateBBox();
        LOGD("     -> Object BBox: " << pPrim->GetBBox()[Geometry::AABBPoint::MIN] <<
             " " << pPrim->GetBBox()[Geometry::AABBPoint::MAX]);
    }

    return true;
}

bool Scene::Load(const std::string& path)
{
    // create default material in case objects have missing materials
    CreateMaterial(Constants::LKRAY_DEFAULT_MATERIAL_NAME, Types::Material::MATTE);

    // load scene from JSON
    std::fstream sceneFile(path, std::fstream::in);
    if (!sceneFile)
    {
        LOGE("Failed to open scene file " << path);
        return false;
    }

    std::string sceneFileString((std::istreambuf_iterator<char>(sceneFile)),
                                std::istreambuf_iterator<char>());

    sceneFile.close();

    rapidjson::Document sceneDesc;
    sceneDesc.Parse(sceneFileString.c_str());

    if (!sceneDesc.IsObject())
    {
        LOGE("File is not a proper JSON object");
        return false;
    }


    // parse scene's name and ambient color
    for (auto& o: sceneDesc.GetObject())
    {
        if (Constants::NAME_NODE_NAME.compare(o.name.GetString()) == 0)
        {
            mName = o.value.GetString();
            break;
        }
    }

    mAmbient = lkCommon::Utils::PixelFloat4(0.0f);
    for (auto& o: sceneDesc.GetObject())
    {
        if (Constants::AMBIENT_NODE_NAME.compare(o.name.GetString()) == 0)
        {
            if (!o.value.IsArray() || (o.value.GetArray().Size() != 3))
            {
                LOGE("Invalid normal parameter for camera. Should be an array of 3 floats.");
                return false;
            }

            uint32_t colIndex = 0;
            for (auto& c: o.value.GetArray())
            {
                mAmbient[colIndex] = c.GetFloat();
                colIndex++;
            }

            mAmbient[3] = 1.0f;

            LOGD(" -> Scene's ambient " << mAmbient);
            break;
        }
    }

    if (mName.empty())
    {
        mName = "UNNAMED";
    }

    LOGI("Loading scene \"" << mName << "\" from path " << path);

    // go through scene description and load materials
    for (auto& o: sceneDesc.GetObject())
    {
        if (Constants::MATERIALS_NODE_NAME.compare(o.name.GetString()) == 0)
        {
            LOGD("Loading materials");
            if (!LoadMaterials(o.value))
            {
                return false;
            }

            break;
        }
    }

    // go again, this time load objects
    for (auto& o: sceneDesc.GetObject())
    {
        if (Constants::OBJECTS_NODE_NAME.compare(o.name.GetString()) == 0)
        {
            LOGD("Loading objects");
            if (!LoadObjects(o.value))
            {
                return false;
            }

            break;
        }
    }

    LOGI("Scene " << mName << " loaded successfully");
    return true;
}

void Scene::BuildBVH()
{
    mBVH.Build();
    mBVH.Print();
}

void Scene::Destroy()
{
    mBVH.Clean();
    mName.clear();
    mMaterials.clear();
}

// -1 if no object hit, otherwise index of hit primitive
Renderer::RayCollision Scene::TestCollision(const Geometry::Ray& ray) const
{
    return mBVH.Traverse(ray);
}

Containers::Ptr<Geometry::Primitive> Scene::CreatePrimitive(const std::string& name, const Types::Primitive& type)
{
    Containers::Ptr<Geometry::Primitive> pPrim;

    switch (type)
    {
    case Types::Primitive::SPHERE:
    {
        pPrim = CreatePtr<Geometry::Primitive, Geometry::Sphere>(name);
        break;
    }
    case Types::Primitive::PLANE:
    {
        pPrim = CreatePtr<Geometry::Primitive, Geometry::Plane>(name);
        break;
    }
    case Types::Primitive::MESH:
    {
        pPrim = CreatePtr<Geometry::Primitive, Geometry::Mesh>(name);
        break;
    }
    default:
    {
        LOGE("Unknown object type");
        return nullptr;
    }
    }

    mBVH.PushObject(pPrim);
    return pPrim;
}

Containers::Ptr<Material::Material> Scene::CreateMaterial(const std::string& name, const Types::Material& type)
{
    Containers::Ptr<Material::Material> pMat;

    auto matIt = mMaterials.find(name);
    if (matIt != mMaterials.end())
    {
        return matIt->second;
    }

    switch (type)
    {
    case Types::Material::MATTE:
    {
        pMat = CreatePtr<Material::Material, Material::Matte>(name);
        break;
    }
    case Types::Material::MIRROR:
    {
        pMat = CreatePtr<Material::Material, Material::Mirror>(name);
        break;
    }
    case Types::Material::EMISSIVE:
    {
        pMat = CreatePtr<Material::Material, Material::Emissive>(name);
        break;
    }
    default:
    {
        LOGE("Unknown material type");
        return nullptr;
    }
    }

    mMaterials.emplace(name, pMat);
    return pMat;
}


} // namespace Scene
} // namespace lkRay
