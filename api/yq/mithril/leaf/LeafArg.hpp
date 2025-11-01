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
    //! Finds leaf, tries key first, then by ID
    Leaf leaf(std::string_view arg_string);
    Leaf leaf(const WebContext&, bool *detected=nullptr);
    Leaf leaf(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Leaf leaf(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    //! Finds leaf by ID only
    Leaf leaf_id(std::string_view arg_string);
    Leaf leaf_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Leaf leaf_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    
    //! Finds leaf by KEY only
    Leaf leaf_key(std::string_view arg_string);
    Leaf leaf_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Leaf leaf_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    //! Finds leaf by TITLE only
    Leaf leaf_title(std::string_view arg_string);
    Leaf leaf_title(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Leaf leaf_title(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}

