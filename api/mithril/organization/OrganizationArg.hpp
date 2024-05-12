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
    //! Finds organization, tries key first, then by ID
    Organization organization(std::string_view arg_string);
    Organization organization(const WebContext&, bool *detected=nullptr);
    Organization organization(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Organization organization(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    //! Finds organization by ID only
    Organization organization_id(std::string_view arg_string);
    Organization organization_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Organization organization_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    
    //! Finds organization by KEY only
    Organization organization_key(std::string_view arg_string);
    Organization organization_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Organization organization_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}

