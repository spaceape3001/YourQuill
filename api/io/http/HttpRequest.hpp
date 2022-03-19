#pragma once
#include <asio/ip/tcp.hpp>
#include <span>
#include <util/net/Http.hpp>
#include <util/net/Url.hpp>
#include <util/net/VersionSpec.hpp>

#include "HttpData.hpp"
#include "HttpHeader.hpp"

namespace yq {

    class HttpConnection;


    class HttpRequest : public RefCount {
        friend class HttpConnection;
    public:
    
        HttpOp                              method() const { return m_method; }
        const UrlView&                      url() const { return m_url; }
        VersionSpec                         version() const { return m_version; }
        const asio::ip::address&            remote() const { return m_remote; }
        const std::vector<std::span<char>>& body() const { return m_body; }
        const std::vector<HttpHeaderView>&  headers() const { return m_headers; }
        
    private:
        size_t                          m_content_length    = 0;
        std::string_view                m_host;
        HttpOp                          m_method;
        int                             m_port  = 0;
        VersionSpec                     m_version;
        UrlView                         m_url;
        std::vector<HttpHeaderView>     m_headers;
        std::vector<HttpDataPtr>        m_buffers;
        std::vector<std::span<char>>    m_body;
        asio::ip::address               m_remote;
    };


}
