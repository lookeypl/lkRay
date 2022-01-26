#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Allocators/Memory.hpp>
#include <lkCommon/Allocators/ArenaAllocator.hpp>

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
    bool mHitInside;

    // to be allocated at a later point
    lkCommon::Allocators::Memory<lkCommon::Allocators::ArenaAllocator>* mAllocator;
    SurfaceDistribution* mSurfaceDistribution;


    RayCollision();
    RayCollision(int hitID, float distance, const lkCommon::Math::Vector4& point,
                 const lkCommon::Math::Vector4& normal, const Geometry::UV& uv,
                 const bool hitInside);
    ~RayCollision();
};

} // namespace Renderer
} // namespace lkRay
