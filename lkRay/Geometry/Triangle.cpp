#include "PCH.hpp"
#include "Triangle.hpp"

namespace lkRay {
namespace Geometry {

Triangle::Triangle(const lkCommon::Math::Vector4& a, const lkCommon::Math::Vector4& b, const lkCommon::Math::Vector4& c)
    : mPoints{a, b, c}
    , mNormal(c - a)
{
    mNormal = mNormal.Cross(b - a).Normalize();
}

bool Triangle::TestCollision(const lkCommon::Math::Vector4& pos, const Ray& ray, float& distance, lkCommon::Math::Vector4& normal)
{
    /*
    // Geometric approach to finding a triangle:
    //   1. Check if ray hits Triangle's plane
    //   2. Check if intersection is inside triangle (inside-outside method)
    // TODO replace with MT algorithm

    float D = mNormal.Dot(mPoints[0] + pos);

    float normalDotDir = mNormal.Dot(ray.GetDirection());
    if (normalDotDir > 0)
        return false;

    distance = -(mNormal.Dot(ray.GetOrigin()) - D) / normalDotDir;
    if (distance < 0)
        return false;

    // we hit a plane, check if we intersect a triangle
    lkCommon::Math::Vector4 p0(mPoints[0] + pos);
    lkCommon::Math::Vector4 p1(mPoints[1] + pos);
    lkCommon::Math::Vector4 p2(mPoints[2] + pos);

    lkCommon::Math::Vector4 collisionPoint(ray.GetOrigin() + ray.GetDirection() * distance);

    lkCommon::Math::Vector4 toCol1(collisionPoint - p0);
    lkCommon::Math::Vector4 v1(toCol1.Cross(p1 - p0));
    lkCommon::Math::Vector4 toCol2(collisionPoint - p1);
    lkCommon::Math::Vector4 v2(toCol2.Cross(p2 - p1));
    lkCommon::Math::Vector4 toCol3(collisionPoint - p2);
    lkCommon::Math::Vector4 v3(toCol3.Cross(p0 - p2));

    if (mNormal.Dot(v1) < 0.0f ||
        mNormal.Dot(v2) < 0.0f ||
        mNormal.Dot(v3) < 0.0f)
        return false;*/

    // MT algorithm
    const float EPSILON = 0.000001f;

    lkCommon::Math::Vector4 dir(ray.GetDirection());
    lkCommon::Math::Vector4 E1 = mPoints[1] - mPoints[0];
    lkCommon::Math::Vector4 E2 = mPoints[2] - mPoints[0];
    lkCommon::Math::Vector4 pv(E2.Cross(dir));
    float d = E1.Dot(pv);

    if (d < EPSILON)
        return false; // backface culling

    float invD = 1.0f / d;

    lkCommon::Math::Vector4 tv(ray.GetOrigin() - mPoints[0] - pos);
    float u = tv.Dot(pv) * invD;
    if (u < 0.0f || u > 1.0f)
        return false;

    lkCommon::Math::Vector4 qv = E1.Cross(tv);
    float v = dir.Dot(qv) * invD;

    if (v < 0.0f || u + v > 1.0f)
        return false;

    distance = E2.Dot(qv) * invD;
    normal = mNormal;
    return true;
}

} // namespace Geometry
} // namespace lkRay
