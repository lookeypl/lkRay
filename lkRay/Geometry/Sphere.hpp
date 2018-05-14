#pragma once

#include "Primitive.hpp"
#include <lkCommon/Math/Vector4.hpp>

namespace lkRay {
namespace Geometry {

class Sphere: public Primitive
{
public:
    Sphere(const lkCommon::Math::Vector4& origin, int r);
    ~Sphere();

    void Intersect();
};

} // namespace Geometry
} // namespace lkRay
