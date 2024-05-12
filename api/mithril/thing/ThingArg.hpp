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
    //! Finds thing, tries key first, then by ID
    Thing thing(std::string_view arg_string);
    Thing thing(const WebContext&, bool *detected=nullptr);
    Thing thing(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Thing thing(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    //! Finds thing by ID only
    Thing thing_id(std::string_view arg_string);
    Thing thing_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Thing thing_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    
    //! Finds thing by KEY only
    Thing thing_key(std::string_view arg_string);
    Thing thing_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Thing thing_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}

