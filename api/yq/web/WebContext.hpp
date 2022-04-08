////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <asio/ip/address.hpp>
#include <yq/net/Http.hpp>
#include <yq/net/Url.hpp>
#include <yq/net/VersionSpec.hpp>
#include <yq/web/HttpData.hpp>
#include <yq/web/HttpHeader.hpp>
#include <time.h>

namespace asio {
    class io_context;
}

namespace yq {
    struct WebContext : public RefCount {
        //! Informational Flags about this... 
        uint32_t                        flags       = 0;

        //! Host of the request
        std::string_view                host;
        
        //! ASIO IO context governing this
        asio::io_context&               io_ctx;
        

        //! Method of the request
        HttpOp                          method;
        
        //! Should be our port, if any is specified
        uint16_t                        port        = 0;

        //! Port they originated from
        uint16_t                        remote_port = 0;

        //! Address of the remote user
        asio::ip::address               remote_addr;

        //! Resolved filename (for extension handlers)
        std::filesystem::path           resolved_file;
        
        //! Spans of data for the received body
        std::vector<std::span<char>>    rx_body;

        //! Length of the RC body
        size_t                          rx_content_length = 0;
        ContentType                     rx_content_type;

        //! Headers received
        std::vector<HttpHeaderView>     rx_headers;
        
        //! Buffers of the received data (don't TOUCH)
        std::vector<HttpDataPtr>        rx_buffers;

        //! Status to reply with
        HttpStatus                      status;

        //! Time of dispatch
        time_t                          time;
        
        struct tm                       timeparts;
        
        //! Time text
        char                            timestamp[64];

        //! Content to transmit
        std::vector<HttpDataPtr>        tx_content;

        //! Type of the data being returned
        ContentType                     tx_content_type;
        
        //! Buffer for the headers going out
        HttpDataPtr                     tx_header_buffer;

        //! URL requested
        UrlView                         url;
        
        //! Path beyond what resolved (ie, for glob patterns)
        std::string_view                truncated_path;

        //! Version of the request
        VersionSpec                     version;
        
        //! Decodes ALL queries
        StringMultiMap                  decode_query();
        
        //! Decodes the first query parameter found by the given name (ignoring the rest)
        std::string                     find_query(std::string_view) const;
        std::string                     find_query(std::initializer_list<std::string_view>) const;
        
        void                            tx_header(std::string_view k, std::string_view v);
        size_t                          tx_content_size() const;

        void                            redirect(std::string_view where, bool permanent=false);
        
        void                            tx_reset(bool resetStatus=false);
        void                            simple_content(std::string_view);
        
        
        enum : uint64_t {
            LOCAL           = 1ULL << 0
        };
        
        WebContext(asio::io_context& _io_ctx);
        ~WebContext();
    };
}
