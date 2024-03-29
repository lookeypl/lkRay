#include "PCH.hpp"
#include "RayCollision.hpp"

#include "SurfaceDistribution.hpp"


namespace lkRay {
namespace Renderer {

RayCollision::RayCollision()
    : mHitID(-1)
    , mDistance(0.0f)
    , mPoint()
    , mNormal()
    , mUV()
    , mSurfaceDistribution(nullptr)
    , mAllocator(nullptr)
{
}

RayCollision::RayCollision(int hitID, float distance, const lkCommon::Math::Vector4& point,
                           const lkCommon::Math::Vector4& normal, const Geometry::UV& uv)
    : mHitID(hitID)
    , mDistance(distance)
    , mPoint(point)
    , mNormal(normal)
    , mUV(uv)
    , mSurfaceDistribution(nullptr)
    , mAllocator(nullptr)
{
}

RayCollision::~RayCollision()
{
    if (mAllocator && mSurfaceDistribution)
    {
        mSurfaceDistribution->~SurfaceDistribution();
        mAllocator->Free(mSurfaceDistribution);
    }
}

} // namespace Renderer
} // namespace lkRay
