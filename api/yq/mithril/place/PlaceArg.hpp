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
    //! Finds place, tries key first, then by ID
    Place place(std::string_view arg_string);
    Place place(const WebContext&, bool *detected=nullptr);
    Place place(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Place place(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    //! Finds place by ID only
    Place place_id(std::string_view arg_string);
    Place place_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Place place_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    
    //! Finds place by KEY only
    Place place_key(std::string_view arg_string);
    Place place_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Place place_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}

