#pragma once

#include "stomplay/header.hpp"
#include "stomplay/method.hpp"
#include "stomplay/ops/fwd.hpp"
#include <stdexcept>
#include <cassert>

#ifndef STOMPLAY_CREATE_BUFFER_RESERVE_SIZE
#define STOMPLAY_CREATE_BUFFER_RESERVE_SIZE 320
#endif

namespace stomplay {

template<class T>
static inline T frame_buffer_create(
    std::size_t reserve = STOMPLAY_CREATE_BUFFER_RESERVE_SIZE)
{
    ops::create_buffer<T> f;
    return f(reserve);
}

template<class T>
class frame
{
    using buffer_type = T;
    buffer_type buffer_{frame_buffer_create<T>()};

public:    
    template<class V>
    constexpr frame(method::ref<V>)
    {
        append_ref(V::text);
    }

    frame(frame&&) = default;
    frame& operator=(frame&&) = default;

    virtual ~frame() = default;

private:
    // all non ref
    void push_header(std::string_view key, std::string_view value)
    {
        if (key.empty())
            throw std::logic_error("header key empty");

        if (value.empty())
            throw std::logic_error("frame header value empty");

        using namespace std::literals;
        append_ref("\n"sv);
        append(key);
        append_ref(":"sv);
        append(value);
    }

    // key ref, val non ref
    void push_prepared_header(std::string_view prepared_key,
        std::string_view value)
    {
        assert(!prepared_key.empty());
        if (value.empty())
            throw std::logic_error("frame header value empty");
        append_ref(prepared_key);
        append(value);
    }

protected:
    template<class V>
    void append(const V& val)
    {
        ops::copy_back<T, V> f;
        f(buffer_, val);
    }

    template<class V>
    void append_ref(V val)
    {
        ops::ref_back<T, V> f;
        f(buffer_, std::move(val));
    }

public:
    // выставить хидер
    template<class K, class V>
    void push(header::base<K, V> hdr)
    {
        push_header(hdr.key(), hdr.value());
    }

    // выставить известный хидер
    // добавляем ключ как ссылку на строку
    template<class K, class V>
    void push(header::known<K, V> hdr)
    {
        push_prepared_header(hdr.key(), hdr.value());
    }

    // выставить известный хидер
    // добавляем ключ как ссылку на строку
    template<class K>
    void push(header::known_ref<K> hdr)
    {
        append_ref(hdr.key_val());
    }

    buffer_type&& content()
    {
        return std::move(buffer_);
    }
};

template<class T>
class body_frame
    : frame<T>
{
    using base_type = frame<T>;

protected:
    using buffer_type = T;
    buffer_type payload_{};
    using base_type::append;
    using base_type::append_ref;

public:
    using base_type::push;

    template<class V>
    constexpr body_frame(V method)
        : base_type{method}
    {   }

    body_frame(body_frame&&) = default;
    body_frame& operator=(body_frame&&) = default;

    virtual ~body_frame() override = default;

    template<class V>
    void push_payload(V val)
    {
        ops::move_back<T, V> f;
        f(payload_, std::move(val));
    }

    void push_payload(const char *data, std::size_t size)
    {
        push_payload(std::string_view{data, size});
    }

    buffer_type&& content()
    {
        using namespace std::literals;
        auto size = payload_.size();
        if (size)
        {
            // дописываем размер
            push(header::content_length(size));

            // разделитель хидеров
            append("\n\n"sv);

            // дописываем протокольный ноль
            push_payload("\0"sv);

            append(std::move(payload_));
        }
        else
            append("\n\n\0"sv);

        return base_type::content();
    }
};

template<class T>
class basic_logon final
    : frame<T>
{
    using base_type = frame<T>;
    using base_type::append;
public:
    using buffer_type = T;
    using base_type::push;

    basic_logon(std::string_view host)
        : base_type{method::connect()}
    {   
        using namespace std::literals;
        push(header::accept_version_v12());
        if (host.empty())
            host = "/"sv;
        push(header::host(host));
    }

    basic_logon(std::string_view host, std::string_view login)
        : basic_logon(host)
    {
        if (!login.empty())
            push(header::login(login));
    }

    basic_logon(std::string_view host, std::string_view login,
        std::string_view passcode)
        : basic_logon(host, login)
    {
        if (!passcode.empty())
            push(header::passcode(passcode));
    }

    buffer_type&& content()
    {
       using namespace std::literals;
       append("\n\n\0"sv);
       return base_type::content();
    }
};

template<class T>
class basic_send final
    : body_frame<T>
{
    using base_type = body_frame<T>;
public:
    using base_type::push;
    using base_type::content;
    using base_type::push_payload;

    basic_send(std::string_view dest)
        : base_type{method::send()}
    {
        push(header::destination(dest));
    }
};

template<class T>
class basic_ack final
    : frame<T>
{
    using base_type = frame<T>;
public:
    using base_type::push;
    using base_type::content;

    basic_ack(std::string_view ack_id)
        : base_type{method::ack()}
    {   
        push(header::id(ack_id));
    }
};

// class subscribe final
//     : public frame
// {
// public:
//     typedef std::function<void(packet)> fn_type;

// private:
//     fn_type fn_{};

// public:
//     subscribe(std::string_view destination, fn_type fn);

//     // возвращает идентификатор подписки
//     std::size_t add_subscribe(subscription_handler& handler);
// };

// class body_frame
//     : public frame
// {
// protected:
//     buffer payload_{};

// public:
//     body_frame() = default;
//     body_frame(body_frame&&) = default;
//     body_frame& operator=(body_frame&&) = default;
//     virtual ~body_frame() = default;

//     void payload(buffer payload);

//     void push_payload(buffer payload);

//     void push_payload(const char *data, std::size_t size);

//     virtual void complete() override;

//     virtual std::string str() const override;
// };

// class ack
//     : public frame
// {
// public:
//     ack(std::string_view ack_id);
// };

// class nack
//     : public frame
// {
// public:
//     nack(std::string_view ack_id);
// };

// class begin
//     : public frame
// {
// public:
//     begin(std::string_view transaction_id);
//     begin(std::size_t transaction_id);
// };

// class commit
//     : public frame
// {
// public:
//     commit(std::string_view transaction_id);
// };

// class abort
//     : public frame
// {
// public:
//     abort(std::string_view transaction_id);
// };

// class receipt
//     : public frame
// {
// public:
//     receipt(std::string_view receipt_id);
// };

// class connected
//     : public frame
// {
// public:
//     connected(std::string_view session, std::string_view server_version);
//     connected(std::string_view session);
// };

// class send
//     : public body_frame
// {
// public:
//     send(std::string_view destination);
// };

// class error
//     : public body_frame
// {
// public:
//     error(std::string_view message, std::string_view receipt_id);
//     error(std::string_view message);
// };

// class message
//     : public body_frame
// {
// public:
//     message(std::string_view destination,
//             std::string_view subscrition, std::string_view message_id);
//     message(std::string_view destination,
//             std::string_view subscrition, std::size_t message_id);
// };

} // namespace stomplay

