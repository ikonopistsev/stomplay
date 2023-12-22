#pragma once

#include "stomplay/ops/fwd.hpp"
#include <vector>
#include <string_view>

namespace stomplay {
namespace ops {

// вставка в конец строки
// всегда будем копировать
template<>
struct copy_back<std::vector<char>, std::string_view>
{
    void operator()(std::vector<char>& out, std::string_view val)
    {
        out.insert(out.end(), val.begin(), val.end());
    }
};

template<>
struct move_back<std::vector<char>, std::string_view>
{
    void operator()(std::vector<char>& out, std::string_view val)
    {
        out.insert(out.end(), val.begin(), val.end());
    }
};

template<>
struct ref_back<std::vector<char>, std::string_view>
{
    void operator()(std::vector<char>& out, std::string_view val)
    {
        out.insert(out.end(), val.begin(), val.end());
    }
};

} // namespace ops
} // namespace stomplay