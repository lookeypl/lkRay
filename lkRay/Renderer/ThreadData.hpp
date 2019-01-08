#pragma once

#include <lkCommon/Utils/ArenaAllocator.hpp>


namespace lkRay {
namespace Renderer {

struct ThreadData
{
    lkCommon::Utils::ArenaAllocator allocator;
    uint32_t rngState;
};

} // namespace lkRay
} // namespace Renderer
