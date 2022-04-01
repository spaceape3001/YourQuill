////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <asio/ip/tcp.hpp>
#include <span>
#include <yq/net/Http.hpp>
#include <yq/net/Url.hpp>
#include <yq/net/VersionSpec.hpp>

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
        const asio::ip::address&            remote_addr() const { return m_remote_addr; }
        int                                 remote_port() const { return m_remote_port; }
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
        asio::ip::address               m_remote_addr;
        int                             m_remote_port   = 0;
    };


}
