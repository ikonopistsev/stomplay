#pragma once

#include "stomplay/header.hpp"
#include "stomplay/method.hpp"
#include <stdexcept>
#include <cassert>

namespace stomplay {

template<class T>
class frame
{
protected:
    using buffer_type = T;
    buffer_type header_{};

public:
    frame() = default;
    virtual ~frame() = default;

    frame(frame&&) = default;
    frame& operator=(frame&&) = default;

    // выставить method
    template<class V>
    void push(method::known_ref<V> method)
    {
        push_method(method.value());
    }

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
        push_header_val(hdr.key(), hdr.value());
    }

    // выставить известный хидер
    // добавляем ключ как ссылку на строку
    template<class K>
    void push(header::known_ref<K> hdr)
    {
        push_header_ref(hdr.key_val());
    }

protected:
    // all non ref
    virtual void push_header(std::string_view key, std::string_view value)
    {
        if (key.empty())
            throw std::logic_error("header key empty");

        if (value.empty())
            throw std::logic_error("frame header value empty");

        using namespace std::literals;
        header_ += "\n"sv;
        header_ += key;
        header_ += ":"sv;
        header_ += value;
    }
    // all ref
    virtual void push_header_ref(std::string_view prepared_key_value)
    {
        assert(!prepared_key_value.empty());
        header_ += prepared_key_value;
    }
    // key ref, val non ref
    virtual void push_header_val(std::string_view prepared_key,
                         std::string_view value)
    {
        assert(!prepared_key.empty());
        if (value.empty())
            throw std::logic_error("frame header value empty");
        header_ += prepared_key;
        header_ += value;
    }

    virtual void push_method(std::string_view method)
    {
        if (method.empty())
            throw std::logic_error("frame method empty");
        header_ += method;
    }

    // complete frame before write
    virtual void complete()
    {
        using namespace std::literals;
        header_ += "\n\n\0"sv;
    }
};

template<class T>
class logon final
    : public frame<T>
{
    using frame<T>::push;
    
public:
    logon(std::string_view host)
    {   
        using namespace std::literals;
        push(method::connect());
        push(header::accept_version_v12());
        if (host.empty())
            host = "/"sv;
        push(header::host(host));
    }

    logon(std::string_view host, std::string_view login)
        : logon(host)
    {
        if (!login.empty())
            push(header::login(login));
    }

    logon(std::string_view host, std::string_view login,
        std::string_view passcode)
        : logon(host, login)
    {
        if (!passcode.empty())
            push(header::passcode(passcode));        
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

