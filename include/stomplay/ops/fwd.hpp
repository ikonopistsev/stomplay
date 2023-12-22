#pragma once

// предекларация функторов

namespace stomplay {
namespace ops {

// подготовка буфера к работе
// резервирование памяти перед работой
template<class T>
struct create_buffer {
    using type = T;
    type operator()(std::size_t reserve) const
    {
        type buf;
        buf.reserve(reserve);
        return buf;
    }
};

// вставка в конец буфера
template<class T, class V>
struct move_back;

template<class T, class V>
struct copy_back
{
    void operator()(T& buf, const V& val)
    {
        buf.insert(buf.end(), val.begin(), val.end());
    }
};

template<class T, class V>
struct ref_back;


} // namespace ops
} // namespace stomplay