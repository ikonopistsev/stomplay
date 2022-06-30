#pragma once

#include <string_view>

namespace stomplay {

constexpr std::string_view version() noexcept
{
#define STOMPLAY_STR_HELPER(x) #x
#define STOMPLAY_STR(x) STOMPLAY_STR_HELPER(x)
    constexpr std::string_view rc = STOMPLAY_STR(STOMPLAY_VERSION);
#undef STOMPLAY_STR_HELPER
#undef STOMPLAY_STR
    return rc;
}

} // namespace stomplay
