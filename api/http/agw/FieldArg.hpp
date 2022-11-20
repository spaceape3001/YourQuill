////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq {
    
    namespace agw {
        struct Field;
    }
    struct WebContext;
    
    namespace arg {
        //! Finds field, tries key first, then by ID
        agw::Field field(std::string_view arg_string);
        agw::Field field(const WebContext&, bool *detected=nullptr);
        agw::Field field(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        agw::Field field(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds field by ID only
        agw::Field field_id(std::string_view arg_string);
        agw::Field field_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        agw::Field field_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds field by KEY only
        agw::Field field_key(std::string_view arg_string);
        agw::Field field_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        agw::Field field_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

