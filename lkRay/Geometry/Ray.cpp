#include "PCH.hpp"
#include "Ray.hpp"

namespace lkRay {
namespace Geometry {

Ray::Ray(const lkCommon::Math::Vector4& origin, const lkCommon::Math::Vector4& direction)
    : mOrigin(origin)
    , mDirection(direction)

{
}

Ray::~Ray()
{
}

} // namespace Geometry
} // namespace lkRay
