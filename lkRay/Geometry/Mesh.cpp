#include "PCH.hpp"
#include "Mesh.hpp"

#include <limits>

#include "Constants.hpp"


namespace lkRay {
namespace Geometry {

Mesh::Mesh(const std::string& name)
    : Primitive(name)
    , mPoints()
    , mTriangleIndices()
{
}

Mesh::Mesh(const std::string& name, const lkCommon::Math::Vector4& pos, const std::vector<lkCommon::Math::Vector4>& points,
           const std::vector<Triangle>& indices)
    : Primitive(name, pos)
    , mPoints(points)
    , mTriangleIndices(indices)
{
}

bool Mesh::TestCollision(const Ray& ray, float& distance, lkCommon::Math::Vector4& normal)
{
    bool hit = false;
    float resDist = 0.0f;
    lkCommon::Math::Vector4 resNorm;
    distance = std::numeric_limits<float>::max();

    // A very naive approach, just goes through all the tris at once.
    // TODO optimizations:
    //  - Calculate AABB of mesh and test against it
    //  - When above passes, use a method to optimize the selection
    for (uint32_t i = 0; i < mTriangleIndices.size(); ++i)
    {
        if (mTriangleIndices[i].TestCollision(mPosition, mPoints, ray, resDist, resNorm))
        {
            if (resDist < distance)
            {
                hit = true;
                distance = resDist;
                normal = resNorm;
            }
        }
    }

    return hit;
}

bool Mesh::ReadParametersFromNode(const rapidjson::Value& value, const Scene::Containers::Material& materials)
{
    if (!Primitive::ReadParametersFromNode(value, materials))
    {
        LOGE("Failed to read base primitive's parameters");
        return false;
    }

    lkCommon::Math::Vector4 vertex;
    Geometry::Triangle tri;

    for (auto& a: value.GetObject())
    {
        if (Constants::MESH_ATTRIBUTE_VERTICES_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray())
            {
                LOGE("Invalid vertices in object " << mName << " - should be an array of 4-element arrays");
                return false;
            }

            for (auto& v: a.value.GetArray())
            {
                if (!v.IsArray() || (v.GetArray().Size() != 4))
                {
                    LOGE("Invalid vertex #" << mPoints.size() - 1 << " in object " << mName);
                    return false;
                }

                vertex = lkCommon::Math::Vector4();

                uint32_t colIndex = 0;
                for (auto& c: v.GetArray())
                {
                    vertex[colIndex] = c.GetFloat();
                    colIndex++;
                }

                mPoints.push_back(vertex);
            }

            LOGD("     -> Mesh has " << mPoints.size() << " vertices");
        }
        else if (Constants::MESH_ATTRIBUTE_INDICES_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            if (!a.value.IsArray())
            {
                LOGE("Invalid indices in object " << mName << " - should be an array");
                return false;
            }

            for (auto& t: a.value.GetArray())
            {
                if (!t.IsArray() || (t.GetArray().Size() != 3))
                {
                    LOGE("Invalid triangle #" << mTriangleIndices.size() - 1 << " in object " << mName);
                    return false;
                }

                tri = Geometry::Triangle();

                uint32_t colIndex = 0;
                for (auto& i: t.GetArray())
                {
                    tri[colIndex] = i.GetUint();
                    colIndex++;
                }

                mTriangleIndices.push_back(tri);
            }

            LOGD("     -> Mesh has " << mTriangleIndices.size() << " triangles");
        }
    }

    return true;
}

} // namespace Geometry
} // namespace lkRay
