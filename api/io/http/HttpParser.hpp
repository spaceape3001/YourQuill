#pragma once
#include "HttpRequest.hpp"
#include "HttpData.hpp"

namespace yq {

    struct MethodUriVersion {
        std::string_view    method, uri, version;
        bool    operator==(const MethodUriVersion&) const = default;
    };

    MethodUriVersion        parse_method_uri(const std::string_view&);

    struct VersionSpec {
        std::string_view    protocol;
        uint16_t            v_major = 0;
        uint16_t            v_minor = 0;
        bool    operator==(const VersionSpec&) const = default;
    };
    
    inline consteval VersionSpec    http09() { return { "http", 0, 9 }; }
    inline consteval VersionSpec    http10() { return { "http", 1, 0 }; }
    inline consteval VersionSpec    http11() { return { "http", 1, 1 }; }
    
    bool                    match(const VersionSpec& a, const VersionSpec& b);

    VersionSpec             parse_version(const std::string_view&);
    
    HttpHeaderView          parse_header_line(const std::string_view&);
    
    UriView                 parse_uri(const std::string_view&);
    

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
