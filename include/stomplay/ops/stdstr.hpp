#pragma once

#include "stomplay/ops/fwd.hpp"
#include <string>
#include <string_view>

namespace stomplay {
namespace ops {

// реализация подготовки буфера
template<>
struct create_buffer<std::string>
{
    using type = std::string;

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
struct copy_back<std::string, std::string_view>
{
    void operator()(std::string& out, std::string_view val)
    {
        out.append(val.begin(), val.end());
    }
};

template<>
struct move_back<std::string, std::string_view>
{
    void operator()(std::string& out, std::string_view val)
    {
        out.append(val.begin(), val.end());
    }
};

template<>
struct ref_back<std::string, std::string_view>
{
    void operator()(std::string& out, std::string_view val)
    {
        out.append(val.begin(), val.end());
    }
};

} // namespace ops
} // namespace stomplay