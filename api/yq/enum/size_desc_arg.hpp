////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

#include "size_desc.hpp"

namespace yq {
    struct WebContext;

    namespace arg {
        SizeDesc    size_desc(std::string_view);
        SizeDesc    size_desc(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        SizeDesc    size_desc(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

