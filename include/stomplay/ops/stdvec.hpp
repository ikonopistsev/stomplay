#pragma once

#include "stomplay/ops/fwd.hpp"
#include <vector>
#include <string_view>

namespace stomplay {
namespace ops {

// реализация подготовки буфера
template<>
struct create_buffer<std::vector<char>>
{
    using type = std::vector<char>;

    type operator()(std::size_t reserve)
    {
        type buf;
        buf.reserve(reserve);
        return buf;
    }
};

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