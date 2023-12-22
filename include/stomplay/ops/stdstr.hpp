#pragma once

#include "stomplay/ops/fwd.hpp"
#include <string>
#include <string_view>

namespace stomplay {
namespace ops {

// вставка в конец строки
// всегда будем копировать
template<class T>
struct copy_back<std::string, T>
{
    void operator()(std::string& out, const T& val)
    {
        out += val;
    }
};

template<class T>
struct move_back<std::string, T>
{
    void operator()(std::string& out, T val)
    {
        out += std::move(val);
    }
};

template<class T>
struct ref_back<std::string, T>
{
    void operator()(std::string& out, const T& val)
    {
        out += val;
    }
};

} // namespace ops
} // namespace stomplay