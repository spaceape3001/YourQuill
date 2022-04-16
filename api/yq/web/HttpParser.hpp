#pragma once

#include <yq/preamble.hpp>
#include <yq/text/KV.hpp>

namespace yq {

    struct MethodUriVersion {
        std::string_view    method, uri, version;
        bool    operator==(const MethodUriVersion&) const = default;
    };

    StringViewMultiMap      parse_parameters(std::string_view);
    MethodUriVersion        parse_method_uri(std::string_view);

    KVView                  parse_header_line(std::string_view);
    StringViewMultiMap      parse_cookie(std::string_view);
    
    
}
