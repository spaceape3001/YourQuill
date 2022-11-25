////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq {
    struct Tag;
    struct WebContext;
    
    namespace arg {
        //! Finds tag, tries key first, then by ID
        Tag tag(std::string_view arg_string);
        Tag tag(const WebContext&, bool *detected=nullptr);
        Tag tag(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Tag tag(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds tag by ID only
        Tag tag_id(std::string_view arg_string);
        Tag tag_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Tag tag_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds tag by KEY only
        Tag tag_key(std::string_view arg_string);
        Tag tag_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Tag tag_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

