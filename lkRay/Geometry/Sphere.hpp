#pragma once

#include "Primitive.hpp"
#include "Math/Vector.hpp"

namespace lkRay {
namespace Geometry {

class Sphere: public Primitive
{
public:
    Sphere(const Math::Vector& origin, int r);
    ~Sphere();

    void Intersect();
};

} // namespace Geometry
} // namespace lkRay
