////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <yq/mithril/preamble.hpp>

namespace yq::mithril::arg {
    //! Finds character, tries key first, then by ID
    Character character(std::string_view arg_string);
    Character character(const WebContext&, bool *detected=nullptr);
    Character character(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Character character(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    //! Finds character by ID only
    Character character_id(std::string_view arg_string);
    Character character_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Character character_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    
    //! Finds character by KEY only
    Character character_key(std::string_view arg_string);
    Character character_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Character character_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}

