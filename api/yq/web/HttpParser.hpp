#pragma once
#include "HttpHeader.hpp"

#include <yq/preamble.hpp>

namespace yq {

    struct MethodUriVersion {
        std::string_view    method, uri, version;
        bool    operator==(const MethodUriVersion&) const = default;
    };

    MethodUriVersion        parse_method_uri(const std::string_view&);

    HttpHeaderView          parse_header_line(const std::string_view&);
}
