////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq {
    struct Class;
    struct WebContext;
    
    namespace arg {
        //! Finds class, tries key first, then by ID
        Class class_(std::string_view arg_string);
        Class class_(const WebContext&, bool *detected=nullptr);
        Class class_(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Class class_(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds class by ID only
        Class class_id(std::string_view arg_string);
        Class class_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Class class_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds class by KEY only
        Class class_key(std::string_view arg_string);
        Class class_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Class class_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

