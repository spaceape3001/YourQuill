////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq::mithril {
    struct Drafter;
    struct WebContext;
    
    namespace arg {
        //! Finds drafter, tries key first, then by ID
        Drafter drafter(std::string_view arg_string);
        Drafter drafter(const WebContext&, bool *detected=nullptr);
        Drafter drafter(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Drafter drafter(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds drafter by ID only
        Drafter drafter_id(std::string_view arg_string);
        Drafter drafter_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Drafter drafter_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds drafter by KEY only
        Drafter drafter_key(std::string_view arg_string);
        Drafter drafter_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Drafter drafter_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

