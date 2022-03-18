#pragma once
#include <asio/ip/tcp.hpp>
#include <span>

namespace yq {

    struct HttpHeaderView {
        std::string_view    key, value;
        
        bool    empty() const;
        bool    operator==(const HttpHeaderView&) const = default;
    };

    template <typename T>
    struct BasicUri {
        T       scheme;
        T       user;
        T       pwd;
        T       host;
        T       path;
        T       query;
        T       fragment;
        int     port    = 0;
        
        bool    operator==(const BasicUri&) const = default;
    };

    using UriView   = BasicUri<std::string_view>;
    using Uri       = BasicUri<std::string>;

    struct HttpRequest {
        std::string_view            method, protocol;
        UriView                     uri;
        std::vector<HttpHeaderView> header;
        std::span<char>             body;
        std::string_view            remote;
        uint16_t                    v_major = 0, v_minor = 9;
    };


}
