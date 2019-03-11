#include "PCH.hpp"
#include "Mesh.hpp"

#include <limits>

#include "Constants.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


namespace {

bool TestTriCollision(uint32_t objID,
                      const lkRay::Scene::Containers::Container<lkRay::Scene::Containers::Ptr<lkRay::Geometry::Triangle>>& objs,
                      const lkRay::Geometry::Ray& ray,
                      float& distance,
                      lkCommon::Math::Vector4& normal)
{
    return objs[objID]->TestCollision(ray, distance, normal);
}

} // namespace

namespace lkRay {
namespace Geometry {

Mesh::Mesh(const std::string& name)
    : Primitive(name)
    , mPoints()
    , mMeshBVH()
{
}

Mesh::Mesh(const std::string& name, const lkCommon::Math::Vector4& pos, const std::vector<lkCommon::Math::Vector4>& points,
           const std::vector<Triangle>& indices)
    : Primitive(name, pos)
    , mPoints(points)
    , mMeshBVH()
{
}

void Mesh::CalculateBBox()
{
    mBBox[AABBPoint::MIN] = lkCommon::Math::Vector4(INFINITY);
    mBBox[AABBPoint::MAX] = lkCommon::Math::Vector4(-INFINITY);

    auto minAssign = [](const float& a, const float& b) -> float {
        return (a < b) ? a : b;
    };
    auto maxAssign = [](const float& a, const float& b) -> float {
        return (a > b) ? a : b;
    };

    for (const auto& p: mPoints)
    {
        mBBox[AABBPoint::MIN][0] = minAssign(p[0], mBBox[AABBPoint::MIN][0]);
        mBBox[AABBPoint::MIN][1] = minAssign(p[1], mBBox[AABBPoint::MIN][1]);
        mBBox[AABBPoint::MIN][2] = minAssign(p[2], mBBox[AABBPoint::MIN][2]);

        mBBox[AABBPoint::MAX][0] = maxAssign(p[0], mBBox[AABBPoint::MAX][0]);
        mBBox[AABBPoint::MAX][1] = maxAssign(p[1], mBBox[AABBPoint::MAX][1]);
        mBBox[AABBPoint::MAX][2] = maxAssign(p[2], mBBox[AABBPoint::MAX][2]);
    }

    mBBox[AABBPoint::MIN][3] = 1.0f;
    mBBox[AABBPoint::MAX][3] = 1.0f;
}

bool Mesh::TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal) const
{
    distance = INFINITY;
    return (mMeshBVH.Traverse(ray, distance, normal) != -1);
}

Types::Primitive Mesh::GetType() const
{
    return Types::Primitive::MESH;
}

bool Mesh::LoadFromFile(const std::string& objFilePath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;
    bool loaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFilePath.c_str());

    if (!warn.empty())
    {
        LOGW("Warnings while parsing OBJ:\n" << warn);
    }

    if (!loaded)
    {
        LOGE("Failed to parse OBJ file " << objFilePath << " for object " << mName);
        if (!err.empty())
        {
            LOGE("Error log:\n" << err);
        }
        return false;
    }

    // TODO avoid this limitation in the future
    if (shapes.size() != 1)
    {
        LOGE("Read invalid number of shapes, should be 1");
        return false;
    }

    tinyobj::mesh_t& m = shapes[0].mesh;

    lkCommon::Math::Vector4 v;
    std::vector<tinyobj::real_t>& objv = attrib.vertices;
    for (uint32_t i = 0; i < objv.size(); i += 3)
    {
        v = lkCommon::Math::Vector4(
            objv[i + 0],
            objv[i + 1],
            objv[i + 2],
            1.0f
        );

        mPoints.push_back(v);
    }

    LOGD("     -> OBJ Mesh has " << mPoints.size() << " vertices.");

    std::vector<tinyobj::index_t>& obji = m.indices;
    for (uint32_t i = 0; i < obji.size(); i += 3)
    {
        mMeshBVH.EmplaceObject(
            mPoints,
            mPosition,
            static_cast<uint32_t>(obji[i + 0].vertex_index),
            static_cast<uint32_t>(obji[i + 1].vertex_index),
            static_cast<uint32_t>(obji[i + 2].vertex_index)
        );
    }

    LOGD("     -> OBJ Mesh has " << mMeshBVH.GetObjectCount() << " triangles.");

    mMeshBVH.Build();
    mMeshBVH.Print();

    return true;
}

bool Mesh::ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials)
{
    if (!Primitive::ReadParametersFromNode(value, materials))
    {
        LOGE("Failed to read base primitive's parameters");
        return false;
    }

    lkCommon::Math::Vector4 vertex;
    bool pathFound = false;
    bool objLoaded = false;
    bool verticesFound = false;
    bool indicesFound = false;

    // first, search if we have an OBJ-based mesh
    for (auto& a: value.GetObject())
    {
        if (Constants::MESH_ATTRIBUTE_PATH_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            pathFound = true;

            if (!a.value.IsString())
            {
                LOGE("Path attribute in object " << mName << " should be a string.");
                break;
            }

            LOGD("     -> Mesh's path is " << a.value.GetString());
            if (!(objLoaded = LoadFromFile(a.value.GetString())))
            {
                LOGE("Failed to read object " << mName << " from OBJ file.");
                break;
            }
        }
    }

    if (pathFound)
    {
        return objLoaded;
    }

    // obj not found - see if we have vert/index combos
    for (auto& a: value.GetObject())
    {
        if (Constants::MESH_ATTRIBUTE_VERTICES_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray())
            {
                LOGE("Invalid vertices in object " << mName << " - should be an array of 3-element arrays");
                return false;
            }

            for (auto& v: a.value.GetArray())
            {
                if (!v.IsArray() || (v.GetArray().Size() != 3))
                {
                    LOGE("Invalid vertex #" << mPoints.size() << " in object " << mName <<
                         ". Should be an array of 3 floats.");
                    return false;
                }

                vertex = lkCommon::Math::Vector4();

                uint32_t colIndex = 0;
                for (auto& c: v.GetArray())
                {
                    vertex[colIndex] = c.GetFloat();
                    colIndex++;
                }

                vertex[3] = 1.0f;

                mPoints.push_back(vertex);
            }

            LOGD("     -> Mesh has " << mPoints.size() << " vertices");
            verticesFound = true;
        }
        else if (Constants::MESH_ATTRIBUTE_INDICES_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray())
            {
                LOGE("Invalid indices parameter in mesh " << mName <<
                     ". Should be an array of 3-element float arrays.");
                return false;
            }

            for (auto& t: a.value.GetArray())
            {
                if (!t.IsArray() || (t.GetArray().Size() != 3))
                {
                    LOGE("Invalid triangle #" << mMeshBVH.GetObjectCount()
                         << " in object " << mName << ". Should be an array of 3 float numbers.");
                    return false;
                }

                uint32_t idx[3];
                uint32_t colIndex = 0;
                for (auto& i: t.GetArray())
                {
                    idx[colIndex] = i.GetUint();
                    colIndex++;
                }

                mMeshBVH.EmplaceObject(mPoints, mPosition, idx[0], idx[1], idx[2]);
            }

            LOGD("     -> Mesh has " << mMeshBVH.GetObjectCount() << " triangles");
            indicesFound = true;
        }
    }

    if (!(verticesFound && indicesFound))
    {
        LOGE("Vertices or indices are missing for mesh " << mName);
        return false;
    }

    mMeshBVH.Build();
    mMeshBVH.Print();

    return true;
}

} // namespace Geometry
} // namespace lkRay
