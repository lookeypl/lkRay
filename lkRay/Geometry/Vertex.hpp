#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <vector>


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

struct Vertex
{
    lkCommon::Math::Vector4 p;
    lkCommon::Math::Vector4 n;
    UV t;
};

using Vertices = std::vector<Vertex>;

} // namespace Geometry
} // namespace lkRay
