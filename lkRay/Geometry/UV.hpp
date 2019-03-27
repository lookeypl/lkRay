#pragma once

namespace lkRay {
namespace Geometry {

struct UV
{
    float u;
    float v;

    UV()
        : u(0.0f)
        , v(0.0f)
    {
    }

    UV(float U, float V)
        : u(U)
        , v(V)
    {
    }
};

} // namespace Geometry
} // namespace lkRay
