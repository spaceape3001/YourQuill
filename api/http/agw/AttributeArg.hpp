////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq {

    namespace agw {
        struct Attribute;
    }
    struct WebContext;

    namespace arg {
        //! Finds attribute by ID
        agw::Attribute attribute(std::string_view arg_string);
        agw::Attribute attribute(const WebContext&, bool *detected=nullptr);
        agw::Attribute attribute(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        agw::Attribute attribute(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
