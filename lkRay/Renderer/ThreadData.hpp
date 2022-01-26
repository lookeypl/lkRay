#pragma once

#include "Constants.hpp"


#include <lkCommon/Allocators/Memory.hpp>
#include <lkCommon/Allocators/ArenaAllocator.hpp>

#include <array>


namespace lkRay {
namespace Renderer {

struct ThreadData
{
    lkCommon::Allocators::Memory<lkCommon::Allocators::ArenaAllocator> allocator;
    uint32_t rngState;
    //Constants::LKRAY_RAY_DEPTH_LIMIT> hitHistory;
};

} // namespace lkRay
} // namespace Renderer
