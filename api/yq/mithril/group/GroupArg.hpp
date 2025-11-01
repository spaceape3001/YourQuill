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
    //! Finds group, tries key first, then by ID
    Group group(std::string_view arg_string);
    Group group(const WebContext&, bool *detected=nullptr);
    Group group(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Group group(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    //! Finds group by ID only
    Group group_id(std::string_view arg_string);
    Group group_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Group group_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    
    //! Finds group by KEY only
    Group group_key(std::string_view arg_string);
    Group group_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Group group_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}

