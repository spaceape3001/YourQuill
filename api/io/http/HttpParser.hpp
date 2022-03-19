#pragma once
#include "HttpRequest.hpp"
#include "HttpData.hpp"
#include "VersionSpec.hpp"

#include <util/preamble.hpp>

namespace yq {

    struct MethodUriVersion {
        std::string_view    method, uri, version;
        bool    operator==(const MethodUriVersion&) const = default;
    };

    MethodUriVersion        parse_method_uri(const std::string_view&);


    VersionSpec             parse_version(const std::string_view&);
    
    HttpHeaderView          parse_header_line(const std::string_view&);
    
    Result<UriView>         parse_uri(const std::string_view&);
    Result<UriView>         parse_hostport(const std::string_view&);
    

#if 0    

    /*!  \brief Http Parsing
    
        Http Parsing is done as a state machine
    */
    class HttpParser {
    public:
        
        enum Status {
            BadSyntax   = 0,
            Partial,
            Done
        };
        
        HttpParser(HttpDataPtr);
        const HttpRequest&  request() const { return m_request; }
        Status              parse(size_t n1, size_t len);
        bool                is_error() const;
        
        
    private:
        HttpDataPtr         m_buffer;
        HttpRequest         m_request;
        const char*         m_str       = nullptr;
        std::string_view    m_key;
        int                 m_mode      = 0;
        
        void                parse(size_t n);
    };
#endif
}
