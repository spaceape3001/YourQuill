#pragma once
#include <asio/ip/tcp.hpp>
#include <span>
#include <util/net/Http.hpp>

#include "HttpData.hpp"
#include "HttpHeader.hpp"
#include "VersionSpec.hpp"

namespace yq {

    class HttpConnection;


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

    class HttpRequest : public RefCount {
        friend class HttpConnection;
    public:
    
        HttpOp                              method() const { return m_method; }
        const UriView&                      uri();
        VersionSpec                         version() const { return m_version; }
        const asio::ip::address&            remote() const { return m_remote; }
        const std::vector<std::span<char>>& body() const { return m_body; }
        
    private:
        size_t                          m_content_length    = 0;
        std::string_view                m_host;
        HttpOp                          m_method;
        int                             m_port  = 0;
        VersionSpec                     m_version;
        UriView                         m_uri;
        std::vector<HttpHeaderView>     m_headers;
        std::vector<HttpDataPtr>        m_buffers;
        std::vector<std::span<char>>    m_body;
        asio::ip::address               m_remote;
    };


}
