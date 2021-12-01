#pragma once

#include <lkCommon/Utils/Pixel.hpp>
#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Allocators/MemoryObject.hpp>
#include <lkCommon/Allocators/ArenaAllocator.hpp>
#include <type_traits>

#include "Renderer/PathContext.hpp"
#include "Renderer/RayCollision.hpp"
#include "Types.hpp"


namespace lkRay {
namespace Distribution {

// Base class for methods of ray distribution on given surface
// gathers both Reflection and Transmission type of distribution
class Function: public lkCommon::Allocators::MemoryObject<lkCommon::Allocators::ArenaAllocator>
{
protected:
    const Types::Distribution mType;

public:
    LKCOMMON_INLINE Function(const Types::Distribution type)
        : mType(type)
    {}

    ~Function() = default;

    virtual lkCommon::Utils::PixelFloat4 F(const Renderer::PathContext& context,
                                           const Renderer::RayCollision& collision,
                                           lkCommon::Math::Vector4& out) = 0;

    LKCOMMON_INLINE bool HasType(const Types::Distribution type) const
    {
        return (mType & type) == type;
    }
};

} // namespace Distribution
} // namespace lkRay
