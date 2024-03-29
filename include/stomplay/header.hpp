#pragma once

#include "stomplay/tag/header.hpp"
#include <string>
#include <chrono>

namespace stomplay {
namespace header {

// TODO странный класс...
// появился set(sv,sv) надобность сомнительна
template<class K, class V>
class base
{
    K key_{};
    V val_{};

public:
    constexpr base(K key) noexcept
        : key_{std::move(key)}
    {   }

    base(K key, V val) noexcept
        : key_{std::move(key)}
        , val_{std::move(val)}
    {   }

    constexpr const K& key() const noexcept
    {
        return key_;
    }

    const V& value() const noexcept
    {
        return val_;
    }
};

template<class K, class V>
constexpr static auto make(K key, V val) noexcept
{
    return base<K, V>(std::move(key), std::move(val));
}

template<class K, class V>
class base_ref
{
    K key_{};
    V val_{};

public:
    constexpr base_ref(K key) noexcept
        : key_{std::move(key)}
    {   }

    constexpr base_ref(K key, V val) noexcept
        : key_{std::move(key)}
        , val_{std::move(val)}
    {   }

    constexpr const K& key() const noexcept
    {
        return key_;
    }

    constexpr const V& value() const noexcept
    {
        return val_;
    }
};

template<class K, class V>
constexpr static auto make_ref(K key, V val) noexcept
{
    return base_ref<K, V>(std::move(key), std::move(val));
}

template<class T, class V>
class known 
    : base<decltype (T::header), V>
{
    using super = base<decltype (T::header), V>;
public:
    constexpr explicit known(V val) noexcept
        : super{T::header, std::move(val)}
    {   }
    using super::key;
    using super::value;
};

template<class T>
class known_ref
{
public:
    using value_type = decltype (T::text);
    value_type key_val_{};

public:
    constexpr known_ref(value_type value)
        : key_val_(value)
    {   }

    constexpr const value_type& key_val() const noexcept
    {
        return key_val_;
    }
};

constexpr static auto content_length(std::string_view val) noexcept
{
    return known<tag::content_length, std::string_view>(val);
}

static inline auto content_length(std::size_t size) noexcept
{
    return known<tag::content_length, std::string>(std::to_string(size));
}

constexpr static auto content_type(std::string_view val) noexcept
{
    return known<tag::content_type, std::string_view>(val);
}

constexpr static auto content_type_json() noexcept {
    return known_ref<tag::content_type>(tag::content_type::header_application_json());
}

constexpr static auto accept_version(std::string_view val) noexcept
{
    return known<tag::accept_version, std::string_view>(val);
}

constexpr static auto host(std::string_view val) noexcept
{
    return known<tag::host, std::string_view>(val);
}

constexpr static auto version(std::string_view val) noexcept
{
    return known<tag::version, std::string_view>(val);
}

constexpr static auto destination(std::string_view val) noexcept
{
    return known<tag::destination, std::string_view>(val);
}

constexpr static auto id(std::string_view val) noexcept
{
    return known<tag::id, std::string_view>(val);
}

constexpr static auto transaction(std::string_view val) noexcept
{
    return known<tag::transaction, std::string_view>(val);
}

static inline auto transaction(std::size_t val) noexcept
{
    return known<tag::transaction, std::string>(std::to_string(val));
}

//// The Stomp message id (not amqp_message_id)
constexpr static auto message_id(std::string_view val) noexcept
{
    return known<tag::message_id, std::string_view>(val);
}

static inline auto message_id(std::size_t val) noexcept
{
    return known<tag::message_id, std::string>(std::to_string(val));
}

constexpr static auto subscription(std::string_view val) noexcept
{
    return known<tag::subscription, std::string_view>(val);
}

constexpr static auto receipt_id(std::string_view val) noexcept
{
    return known<tag::receipt_id, std::string_view>(val);
}

constexpr static auto login(std::string_view val) noexcept
{
    return known<tag::login, std::string_view>(val);
}

constexpr static auto passcode(std::string_view val) noexcept
{
    return known<tag::passcode, std::string_view>(val);
}

constexpr static auto heart_beat(std::string_view val) noexcept
{
    return known<tag::heart_beat, std::string_view>(val);
}

constexpr static auto session(std::string_view val) noexcept
{
    return known<tag::session, std::string_view>(val);
}

constexpr static auto server(std::string_view val) noexcept
{
    return known<tag::server, std::string_view>(val);
}

constexpr static auto ack(std::string_view val) noexcept
{
    return known<tag::ack, std::string_view>(val);
}

constexpr static auto receipt(std::string_view val) noexcept
{
    return known<tag::receipt, std::string_view>(val);
}

// The ERROR frame SHOULD contain a message header
// with a short description of the error
//typedef basic<tag::message> message;
constexpr static auto message(std::string_view val) noexcept
{
    return known<tag::message, std::string_view>(val);
}
//typedef basic<tag::prefetch_count> prefetch_count;
constexpr static auto prefetch_count(std::string_view val) noexcept
{
    return known<tag::prefetch_count, std::string_view>(val);
}

static inline auto prefetch_count(std::size_t val) noexcept
{
    return known<tag::prefetch_count, std::string>(std::to_string(val));
}
//typedef basic<tag::durable> durable;
constexpr static auto durable(std::string_view val) noexcept
{
    return known<tag::durable, std::string_view>(val);
}
//typedef basic<tag::auto_delete> auto_delete;
constexpr static auto auto_delete(std::string_view val) noexcept
{
    return known<tag::auto_delete, std::string_view>(val);
}

//typedef basic<tag::message_ttl> message_ttl;
constexpr static auto message_ttl(std::string_view val) noexcept
{
    return known<tag::message_ttl, std::string_view>(val);
}

static inline auto message_ttl(std::size_t val) noexcept
{
    return known<tag::message_ttl, std::string>(std::to_string(val));
}

template<class Rep, class Period>
static auto message_ttl(std::chrono::duration<Rep, Period> timeout) noexcept
{
    using namespace std::chrono;
    auto time = duration_cast<milliseconds>(timeout).count();
    return message_ttl(static_cast<std::size_t>(time));
}

//typedef basic<tag::expires> expires;
constexpr static auto expires(std::string_view val) noexcept
{
    return known<tag::expires, std::string_view>(val);
}

static inline auto expires(std::size_t val) noexcept
{
    return known<tag::expires, std::string>(std::to_string(val));
}

template<class Rep, class Period>
static auto expires(std::chrono::duration<Rep, Period> timeout) noexcept
{
    using namespace std::chrono;
    auto time = duration_cast<milliseconds>(timeout).count();
    return expires(static_cast<std::size_t>(time));
}

//typedef basic<tag::max_length> max_length;
constexpr static auto max_length(std::string_view val) noexcept
{
    return known<tag::max_length, std::string_view>(val);
}

static inline auto max_length(std::size_t val) noexcept
{
    return known<tag::max_length, std::string>(std::to_string(val));
}

//typedef basic<tag::max_length_bytes> max_length_bytes;
constexpr static auto max_length_bytes(std::string_view val) noexcept
{
    return known<tag::max_length_bytes, std::string_view>(val);
}

static inline auto max_length_bytes(std::size_t val) noexcept
{
    return known<tag::max_length_bytes, std::string>(std::to_string(val));
}

//typedef basic<tag::dead_letter_exchange> dead_letter_exchange;
constexpr static auto dead_letter_exchange(std::string_view val) noexcept
{
    return known<tag::dead_letter_exchange, std::string_view>(val);
}

//typedef basic<tag::dead_letter_routing_key> dead_letter_routing_key;
constexpr static auto dead_letter_routing_key(std::string_view val) noexcept
{
    return known<tag::dead_letter_routing_key, std::string_view>(val);
}

//typedef basic<tag::max_priority> max_priority;
constexpr static auto max_priority(std::string_view val) noexcept
{
    return known<tag::max_priority, std::string_view>(val);
}

//typedef basic<tag::persistent> persistent;
constexpr static auto persistent(std::string_view val) noexcept
{
    return known<tag::persistent, std::string_view>(val);
}

//typedef basic<tag::reply_to> reply_to;
constexpr static auto reply_to(std::string_view val) noexcept
{
    return known<tag::reply_to, std::string_view>(val);
}

//typedef basic<tag::redelivered> redelivered;
constexpr static auto redelivered(std::string_view val) noexcept
{
    return known<tag::redelivered, std::string_view>(val);
}

//typedef basic<tag::original_exchange> original_exchange;
constexpr static auto original_exchange(std::string_view val) noexcept
{
    return known<tag::original_exchange, std::string_view>(val);
}

//typedef basic<tag::original_routing_key> original_routing_key;
constexpr static auto original_routing_key(std::string_view val) noexcept
{
    return known<tag::original_routing_key, std::string_view>(val);
}

//typedef basic<tag::queue_name> queue_name;
constexpr static auto queue_name(std::string_view val) noexcept
{
    return known<tag::queue_name, std::string_view>(val);
}

//typedef basic<tag::queue_type> queue_type;
constexpr static auto queue_type(std::string_view val) noexcept
{
    return known<tag::queue_type, std::string_view>(val);
}

//typedef basic<tag::content_encoding> content_encoding;
constexpr static auto content_encoding(std::string_view val) noexcept
{
    return known<tag::content_encoding, std::string_view>(val);
}

//typedef basic<tag::priority> priority;
constexpr static auto priority(std::string_view val) noexcept
{
    return known<tag::priority, std::string_view>(val);
}

//typedef basic<tag::correlation_id> correlation_id;
constexpr static auto correlation_id(std::string_view val) noexcept
{
    return known<tag::correlation_id, std::string_view>(val);
}

//typedef basic<tag::expiration> expiration;
constexpr static auto expiration(std::string_view val) noexcept
{
    return known<tag::expiration, std::string_view>(val);
}

//typedef basic<tag::amqp_message_id> amqp_message_id;
constexpr static auto amqp_message_id(std::string_view val) noexcept
{
    return known<tag::amqp_message_id, std::string_view>(val);
}

static inline auto amqp_message_id(std::size_t val) noexcept
{
    return known<tag::amqp_message_id, std::string>(std::to_string(val));
}

//typedef basic<tag::timestamp> timestamp;
constexpr static auto timestamp(std::string_view val) noexcept
{
    return known<tag::timestamp, std::string_view>(val);
}

//typedef basic<tag::timestamp> timestamp;
static inline auto timestamp(std::size_t val) noexcept
{
    return known<tag::timestamp, std::string>(std::to_string(val));
}

template<class Rep, class Period>
static auto timestamp(std::chrono::duration<Rep, Period> timeout) noexcept
{
    using namespace std::chrono;
    auto time = duration_cast<milliseconds>(timeout).count();
    return timestamp(static_cast<std::size_t>(time));
}

static inline auto time_since_epoch() noexcept
{
    return timestamp(std::chrono::system_clock::now().time_since_epoch());
}

//typedef basic<tag::amqp_type> amqp_type;
constexpr static auto amqp_type(std::string_view val) noexcept
{
    return known<tag::amqp_type, std::string_view>(val);
}

//typedef basic<tag::user_id> user_id;
constexpr static auto user_id(std::string_view val) noexcept
{
    return known<tag::user_id, std::string_view>(val);
}

//typedef basic<tag::app_id> app_id;
constexpr static auto app_id(std::string_view val) noexcept
{
    return known<tag::app_id, std::string_view>(val);
}
//typedef basic<tag::cluster_id> cluster_id;
constexpr static auto cluster_id(std::string_view val) noexcept
{
    return known<tag::cluster_id, std::string_view>(val);
}

constexpr static auto accept_version_v12() noexcept {
    return known_ref<tag::accept_version>(tag::accept_version::header_v12());
}

constexpr static auto version_v12() noexcept {
    return known_ref<tag::version>(tag::version::header_v12());
}

constexpr static auto persistent_on() noexcept {
    return known_ref<tag::persistent>(tag::persistent::header_enable());
}

} // namespace header
} // namespace stomptalk
