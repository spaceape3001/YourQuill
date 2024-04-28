////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq::mithril {
    struct Doodle;
    struct WebContext;
    
    namespace arg {
        //! Finds doodle, tries key first, then by ID
        Doodle doodle(std::string_view arg_string);
        Doodle doodle(const WebContext&, bool *detected=nullptr);
        Doodle doodle(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Doodle doodle(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds doodle by ID only
        Doodle doodle_id(std::string_view arg_string);
        Doodle doodle_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Doodle doodle_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds doodle by KEY only
        Doodle doodle_key(std::string_view arg_string);
        Doodle doodle_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Doodle doodle_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

