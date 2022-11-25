////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq {
    struct Field;
    struct WebContext;
    
    namespace arg {
        //! Finds field, tries key first, then by ID
        Field field(std::string_view arg_string);
        Field field(const WebContext&, bool *detected=nullptr);
        Field field(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Field field(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds field by ID only
        Field field_id(std::string_view arg_string);
        Field field_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Field field_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds field by KEY only
        Field field_key(std::string_view arg_string);
        Field field_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Field field_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

