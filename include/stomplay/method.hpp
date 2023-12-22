#pragma once

#include "stomplay/tag/method.hpp"
#include <vector>

namespace stomplay {
namespace method {

template<class T>
struct ref
{   };

constexpr static auto ack() noexcept
{
    return ref<tag::ack>();
}

constexpr static auto nack() noexcept
{
    return ref<tag::nack>();
}

constexpr static auto send() noexcept
{
    return ref<tag::send>();
}

constexpr static auto abort() noexcept
{
    return ref<tag::abort>();
}

constexpr static auto begin() noexcept
{
    return ref<tag::begin>();
}

constexpr static auto error() noexcept
{
    return ref<tag::error>();
}

constexpr static auto stomp() noexcept
{
    return ref<tag::stomp>();
}

constexpr static auto commit() noexcept
{
    return ref<tag::commit>();
}

constexpr static auto connect() noexcept
{
    return ref<tag::connect>{};
}

constexpr static auto message() noexcept
{
    return ref<tag::message>();
}

constexpr static auto receipt() noexcept
{
    return ref<tag::receipt>();
}

constexpr static auto subscribe() noexcept
{
    return ref<tag::subscribe>();
}

constexpr static auto connected() noexcept
{
    return ref<tag::connected>();
}

constexpr static auto disconnect() noexcept
{
    return ref<tag::disconnect>();
}

constexpr static auto unsubscribe() noexcept
{
    return ref<tag::unsubscribe>();
}

} // namespace method
} // namespace stomptalk
