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
        add_back_ref(V::text);
    }

    frame(frame&&) = default;
    frame& operator=(frame&&) = default;

    virtual ~frame() = default;

protected:
    void move_back(auto val)
    {
        ops::move_back<T, decltype(val)> f;
        f(buffer_, std::move(val));
    }

    template<class V>
    void add_back(const V& val)
    {
        ops::copy_back<T, V> f;
        f(buffer_, val);
    }

    template<class V>
    void add_back_ref(const V& val)
    {
        ops::ref_back<T, V> f;
        f(buffer_, val);
    }

    static void value_not_empty(const auto& value)
    {
        if (value.empty())
            throw std::logic_error("header value empty");
    }

    static void header_not_empty(const auto& key, const auto& value)
    {
        if (key.empty())
            throw std::logic_error("header key empty");

        value_not_empty(value);
    }

public:
    buffer_type& content() noexcept
    {
        return buffer_;
    }

public:
    void set(std::string_view key, std::string_view value)
    {
        header_not_empty(key, value);

        using namespace std::literals;
        add_back_ref("\n"sv);
        add_back(key);
        add_back_ref(":"sv);
        add_back(value);
    }

    // выставить хидер
    template<class K, class V>
    void set(header::base<K, V> hdr)
    {
        set(hdr.key(), hdr.value());
    }

    // выставить хидер
    template<class K, class V>
    void set(header::base_ref<K, V> hdr)
    {
        auto& key = hdr.key();
        auto& value = hdr.value();
        header_not_empty(key, value);

        using namespace std::literals;
        add_back_ref("\n"sv);
        add_back_ref(key);
        add_back_ref(":"sv);
        add_back_ref(value);
    }    

    // выставить известный хидер
    // добавляем ключ как ссылку на строку
    template<class K, class V>
    void set(header::known<K, V> hdr)
    {
        auto& prepared_key = hdr.key();
        auto& value = hdr.value();        
        assert(!prepared_key.empty());

        value_not_empty(value);

        add_back_ref(prepared_key);
        add_back(value);
    }

    // выставить известный хидер
    // добавляем ключ как ссылку на строку
    template<class K>
    void set(header::known_ref<K> hdr)
    {
        add_back_ref(hdr.key_val());
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
    using base_type::add_back_ref;
    using base_type::move_back;

public:
    using base_type::set;

    constexpr body_frame(auto method)
        : base_type{method}
    {   }

    body_frame(body_frame&&) = default;
    body_frame& operator=(body_frame&&) = default;

    virtual ~body_frame() override = default;

    void push(auto val)
    {
        ops::move_back<T, decltype(val)> f;
        f(payload_, std::move(val));
    }

    template<class V>
    void push_ref(const V& val)
    {
        ops::ref_back<T, V> f;
        f(payload_, val);
    }

    template<class V>   
    void copy(const V& val)
    {
        ops::copy_back<T, V> f;
        f(payload_, val);
    }

    void push_payload(auto val)
    {
        push(std::move(val));
    }

    void copy_payload(const auto& val)
    {
        copy(val);
    }

    void push_payload_ref(const auto& val)
    {
        push_ref(val);
    }

    buffer_type& content()
    {
        using namespace std::literals;
        auto size = payload_.size();
        if (size)
        {
            // дописываем размер
            set(header::content_length(size));

            // разделитель хидеров
            add_back_ref("\n\n"sv);

            // дописываем протокольный ноль в payload
            push_ref("\0"sv);

            // объединяем хидеры и payload
            move_back(std::move(payload_));
        }
        else
            add_back_ref("\n\n\0"sv);

        return base_type::content();
    }
};

template<class T>
class basic_logon final
    : frame<T>
{
    using base_type = frame<T>;
    using base_type::content;
    using base_type::add_back_ref;
public:
    using buffer_type = T;
    using base_type::set;

    basic_logon(std::string_view host)
        : base_type{method::connect()}
    {   
        using namespace std::literals;
        set(header::accept_version_v12());
        if (host.empty())
            host = "/"sv;
        set(header::host(host));
    }

    basic_logon(std::string_view host, std::string_view login)
        : basic_logon(host)
    {
        if (!login.empty())
            set(header::login(login));
    }

    basic_logon(std::string_view host, std::string_view login,
        std::string_view passcode)
        : basic_logon(host, login)
    {
        if (!passcode.empty())
            set(header::passcode(passcode));
    }

    buffer_type& content()
    {
       using namespace std::literals;
       add_back_ref("\n\n\0"sv);
       return base_type::content();
    }
};

template<class T>
class basic_send final
    : body_frame<T>
{
    using base_type = body_frame<T>;
public:
    using base_type::set;
    using base_type::push;
    using base_type::content;

    basic_send(std::string_view dest)
        : base_type{method::send()}
    {
        set(header::destination(dest));
    }
};

template<class T>
class basic_ack final
    : frame<T>
{
    using base_type = frame<T>;
public:
    using base_type::set;
    using base_type::content;

    basic_ack(std::string_view ack_id)
        : base_type{method::ack()}
    {   
        set(header::id(ack_id));
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

