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
    //! Finds entity, tries key first, then by ID
    Entity entity(std::string_view arg_string);
    Entity entity(const WebContext&, bool *detected=nullptr);
    Entity entity(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Entity entity(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    //! Finds entity by ID only
    Entity entity_id(std::string_view arg_string);
    Entity entity_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Entity entity_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    
    //! Finds entity by KEY only
    Entity entity_key(std::string_view arg_string);
    Entity entity_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Entity entity_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}

