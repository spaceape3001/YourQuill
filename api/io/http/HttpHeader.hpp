#pragma once

#include <string>
#include <string_view>

namespace yq {
    template <typename T>
    struct BasicHttpHeader {
        T       key, value;
        bool    empty() const { return key.empty() && value.empty(); }
        bool    operator==(const BasicHttpHeader&) const = default;
    };
    
    using  HttpHeaderView  = BasicHttpHeader<std::string_view>;
    using  HttpHeader      = BasicHttpHeader<std::string>;
}
