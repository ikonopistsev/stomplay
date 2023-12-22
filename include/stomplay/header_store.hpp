#pragma once

#include "stomplay/header.hpp"

#include <unordered_map>
#include <cassert>

namespace stomplay {

class header_store
{
    using hash_type = fnv1a::type;
    using version_type = std::size_t;
    using value_type = std::tuple<std::string, std::string, version_type>;
    using header_type = std::pair<std::string_view, std::string_view>;
    using storage_type = std::unordered_map<hash_type, value_type>;

    version_type version_{};
    storage_type storage_{};

public:
    header_store() = default;

    void set(hash_type num_id, std::string_view key, std::string_view value)
    {
        auto f = storage_.find(num_id);
        if (f != storage_.end())
        {
            auto& [ storage_key, storage_val, 
                storage_version ] = std::get<1>(*f);
            assert(storage_key == key);
            if (storage_version != version_)
            {
                storage_val = value;
                storage_version = version_;
            }
        }
        else
        {
            assert(stomptalk::fnv1a::calc_hash<decltype(key)>(key.begin(), 
                key.end()) == num_id);
            storage_[num_id] = std::make_tuple(std::string{key}, 
                std::string{value}, version_);
        }
    }

    void set(std::string_view key, std::string_view value)
    {
       fnv1a h;
       auto text_hash = h(key.begin(), key.end());
       set(text_hash, key, value);
    }

    void set(const header_type& hdr)
    {
        auto& [ key, value ] = hdr;
        set(key, value);
    }

    template<class T>
    void set(std::string_view value)
    {
        set(T::text_hash, T::text, value);
    }

    template<class T, class V>
    void set(const header::known<T, V>& header)
    {
        set(T::text_hash, T::text, header.value());
    }

    void clear() noexcept
    {
        ++version_;
    }

    template<class F>
    std::string_view find(hash_type num_id, F fn) const
    {
        auto f = storage_.find(num_id);
        if (f != storage_.end())
        {
            const auto& [ storage_key, storage_val, 
                storage_version ] = std::get<1>(*f);
            if (storage_version == version_)
            {
                return fn(std::make_pair(std::string_view{storage_key}, 
                    std::string_view{storage_val}));
            } 
        }
        return {};
    }

    std::string_view get(std::string_view key) const noexcept
    {
        fnv1a h;
        return find(h(key.begin(), key.end()), [&](auto hdr) {
            auto& [ hdr_key, hdr_val ] = hdr;
            assert(hdr_key == key);
            return hdr_val;
        });
    }

    std::string_view get(hash_type num_id) const noexcept
    {
        return find(num_id, [&](auto hdr) {
            return std::get<1>(hdr);
        });
    }

    template<class T>
    std::string_view get() const noexcept
    {
        return get(T::text_hash);
    }

    std::string dump(char sep = ' ') const
    {
       std::string rc;
       rc.reserve(320);

       for (auto& [num_id, data] : storage_)
       {
            const auto& [ kev, value, version ] = data;
            if (version == version_)
            {
                if (!rc.empty())
                    rc += sep;

                rc += '\"';
                rc += kev;
                rc += '\"';
                rc += ':';
                rc += '\"';
                rc += value;
                rc += '\"';
                rc += ' ';
                rc += std::to_string(num_id);
            }
       }

       return rc;
    }
};

} // namespace stomplay
