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
    //! Finds event, tries key first, then by ID
    Event event(std::string_view arg_string);
    Event event(const WebContext&, bool *detected=nullptr);
    Event event(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Event event(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

    //! Finds event by ID only
    Event event_id(std::string_view arg_string);
    Event event_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Event event_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    
    //! Finds event by KEY only
    Event event_key(std::string_view arg_string);
    Event event_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
    Event event_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
}

