#pragma once
#include <asio/ip/tcp.hpp>
#include <span>

namespace yq {

    struct HttpHeaderView {
        std::string_view    key, value;
        
        bool    empty() const;
        bool    operator==(const HttpHeaderView&) const = default;
    };

    struct UriBits {
        std::string_view            scheme, host, user, pass, path, query, port, fragment;
    };

    template <typename T>
    struct BasicUri {
        T       scheme;
        T       host;
        T       path;
        T       user;
        T       pwd;
        T       query;
        T       fragment;
        int     port    = 0;
    };

    using UriView   = BasicUri<std::string_view>;

    struct HttpRequest {
        std::string_view            method, protocol;
        UriView                     uri;
        std::vector<HttpHeaderView> header;
        std::span<char>             body;
        std::string_view            remote;
        uint16_t                    v_major = 0, v_minor = 9;
    };


}
