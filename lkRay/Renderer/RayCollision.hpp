#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/ArenaAllocator.hpp>

#include "Prerequisites.hpp"
#include "Geometry/UV.hpp"


namespace lkRay {
namespace Renderer {

// gathers information about current collision point
class RayCollision
{
public:
    int32_t mHitID;
    float mDistance;
    lkCommon::Math::Vector4 mPoint;
    lkCommon::Math::Vector4 mNormal;
    Geometry::UV mUV;

    // to be allocated at a later point
    lkCommon::Utils::ArenaAllocator* mAllocator;
    Renderer::SurfaceDistribution* mSurfaceDistribution;


    RayCollision();
    RayCollision(int hitID, float distance, const lkCommon::Math::Vector4& point,
                 const lkCommon::Math::Vector4& normal, const Geometry::UV& uv);
    ~RayCollision();
};

} // namespace Renderer
} // namespace lkRay
