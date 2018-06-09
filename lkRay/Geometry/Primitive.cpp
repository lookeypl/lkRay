#include "PCH.hpp"
#include "Primitive.hpp"

namespace lkRay {
namespace Geometry {

Primitive::Primitive()
    : mPosition(0.0f, 0.0f, 0.0f, 1.0f)
{
}

Primitive::Primitive(const lkCommon::Math::Vector4& position)
    : mPosition(position)
{
}

} // namespace Geometry
} // namespace lkRay
