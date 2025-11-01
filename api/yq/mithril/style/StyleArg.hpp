////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq::mithril {
    struct Style;
    struct WebContext;
    
    namespace arg {
        //! Finds style, tries key first, then by ID
        Style style(std::string_view arg_string);
        Style style(const WebContext&, bool *detected=nullptr);
        Style style(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Style style(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds style by ID only
        Style style_id(std::string_view arg_string);
        Style style_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Style style_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds style by KEY only
        Style style_key(std::string_view arg_string);
        Style style_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Style style_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

