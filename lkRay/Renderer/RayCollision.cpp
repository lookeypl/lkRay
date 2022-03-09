#include "PCH.hpp"
#include "RayCollision.hpp"

#include "SurfaceDistribution.hpp"


namespace lkRay {
namespace Renderer {

RayCollision::RayCollision()
    : mHitID(-1)
    , mDistance(0.0f)
    , mRay()
    , mNormal()
    , mUV()
    , mPoint()
    , mSurfaceDistribution(nullptr)
    , mAllocator(nullptr)
{
}

RayCollision::RayCollision(int32_t hitID, float distance, const Geometry::Ray& ray,
                           const lkCommon::Math::Vector4& normal, const Geometry::UV& uv)
    : mHitID(hitID)
    , mDistance(distance)
    , mRay(ray)
    , mNormal(normal)
    , mUV(uv)
    , mPoint()
    , mSurfaceDistribution(nullptr)
    , mAllocator(nullptr)
{
    if (mNormal.Dot(mRay.mDirection) > 0.0f)
        mNormal *= -1.0f;

    mPoint = mRay.mOrigin + mRay.mDirection * mDistance;
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
