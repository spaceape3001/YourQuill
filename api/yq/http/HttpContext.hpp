#pragma once

#include <functional>
#include <asio.hpp>
#include <yq/collection/EnumMap.hpp>
#include <yq/net/Http.hpp>
#include <yq/net/VersionSpec.hpp>

namespace yq {
    struct HttpRequest;
    class HttpResponse;
    
    using HttpExecutor      = std::function<void(const HttpRequest&, HttpResponse&)>;
    using StatusMessages    = EnumMap<HttpStatus, std::string>;
    
    
    struct HttpContext {
        asio::io_context&       io_ctx;
        VersionSpec             version = http10();
        HttpExecutor            handler;
        uint16_t                port = 0;
        StatusMessages          messages;
        
        HttpContext(asio::io_context&);
    };
    
}
