////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

#include <mithril/preamble.hpp>

namespace yq::mithril::arg {
    //! Finds attribute by ID
    Attribute attribute(std::string_view arg_string);
    Attribute attribute(const WebContext&, bool *detected=nullptr);
    Attribute attribute(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Attribute attribute(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}
