////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq::mithril {
    struct Novel;
    struct WebContext;
    
    namespace arg {
        //! Finds novel, tries key first, then by ID
        Novel novel(std::string_view arg_string);
        Novel novel(const WebContext&, bool *detected=nullptr);
        Novel novel(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Novel novel(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds novel by ID only
        Novel novel_id(std::string_view arg_string);
        Novel novel_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Novel novel_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds novel by KEY only
        Novel novel_key(std::string_view arg_string);
        Novel novel_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Novel novel_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds novel by TITLE only
        Novel novel_title(std::string_view arg_string);
        Novel novel_title(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Novel novel_title(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

