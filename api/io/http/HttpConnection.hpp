#pragma once

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <array>
#include <memory>
#include <vector>

#include <util/net/Http.hpp>

#include "HttpData.hpp"
#include "HttpRequest.hpp"

namespace yq {
    struct HttpRequest;
    class HttpResponse;

    struct HttpContext;

    class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
    public:
        
        HttpConnection(HttpContext&, asio::ip::tcp::socket);
        ~HttpConnection();
        
        void        start() { do_read(); }
    
    private:
    
        void        do_read();
        void        process();

        enum RxMode {
            RxMethod    = 0,
            RxHeader,
            RxBody,
            RxDispatch,
            RxDone,
            RxError
        };
        
        HttpContext&            m_context;
        asio::ip::tcp::socket   m_socket;
        HttpDataPtr             m_buffer;   // Rx buffer... response is extra
        RxMode                  m_rxMode = RxMethod;
        size_t                  m_next  = 0;
        Ref<HttpRequest>        m_request;
        bool                    m_eod   = false;
        
        void        rxMethod();
        void        rxHeader();
        void        rxBody();
        void        rxDispatch();
        
        
        void        dispatch(Ref<HttpRequest>);
        void        dispatch(HttpStatus code);
        void        dispatch(HttpStatus code, VersionSpec);
        void        send(Ref<HttpResponse>);
        VersionSpec         limit(VersionSpec) const;
        std::string_view    next_line();
    };
}
