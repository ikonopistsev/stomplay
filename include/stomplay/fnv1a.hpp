// Copyright © 2020 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "stomptalk/fnv1a.hpp"

namespace stomplay {

template<class T, stomptalk::fnv1a::type H>
struct static_hash
{
    constexpr static auto value = 
        stomptalk::fnv1a::calc_hash<decltype(T::text)>(T::text.begin(), T::text.end());
    using enable_type = 
        typename std::enable_if<value == H, std::nullptr_t>::type;
};

} // namespace stomplay
