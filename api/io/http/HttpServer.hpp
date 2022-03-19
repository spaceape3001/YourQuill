#pragma once

#include <asio.hpp>
#include <asio/ts/internet.hpp>

namespace yq {
    class HttpServer {
    public:

        HttpServer(HttpContext&);
        ~HttpServer();
        void            start();
        
    private:
        HttpContext&                m_context;
        asio::ip::tcp::acceptor     m_acceptor;
        asio::ip::tcp::socket       m_socket;
    };
}
