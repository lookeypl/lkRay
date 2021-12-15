#pragma once

#include "Prerequisites.hpp"

#include <map>
#include <string>
#include <vector>

#include <lkCommon/Utils/Pixel.hpp>

#include "Containers.hpp"
#include "Types.hpp"

#include "BVH.hpp"

#include "Renderer/RayCollision.hpp"
#include "Geometry/Ray.hpp"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>


namespace lkRay {
namespace Scene {

class Scene final
{
private:
    Containers::Material mMaterials;
    lkCommon::Utils::PixelFloat4 mAmbient;
    std::string mName;
    BVH<Containers::Ptr<Geometry::Primitive>> mBVH;

    bool LoadMaterials(const rapidjson::Value& node);
    bool LoadObjects(const rapidjson::Value& node);

public:
    Scene();
    ~Scene();

    bool Load(const std::string& path);
    void BuildBVH();
    void Destroy();

    Renderer::RayCollision TestCollision(const Geometry::Ray& ray) const;

    Containers::Ptr<Geometry::Primitive> CreatePrimitive(const std::string& name, const Types::Primitive& type);
    Containers::Ptr<Material::Material> CreateMaterial(const std::string& name, const Types::Material& type);


    LKCOMMON_INLINE void SetAmbient(const lkCommon::Utils::PixelFloat4 ambient)
    {
        mAmbient = ambient;
    }


    LKCOMMON_INLINE const Containers::Primitive& GetPrimitives() const
    {
        return mBVH.GetPrimitives();
    }

    LKCOMMON_INLINE const lkCommon::Utils::PixelFloat4& GetAmbient() const
    {
        return mAmbient;
    }
};

} // namespace Scene
} // namespace lkRay
