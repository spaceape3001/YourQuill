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
        struct Class;
    }
    struct WebContext;
    
    namespace arg {
        //! Finds class, tries key first, then by ID
        agw::Class class_(std::string_view arg_string);
        agw::Class class_(const WebContext&, bool *detected=nullptr);
        agw::Class class_(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        agw::Class class_(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds class by ID only
        agw::Class class_id(std::string_view arg_string);
        agw::Class class_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        agw::Class class_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds class by KEY only
        agw::Class class_key(std::string_view arg_string);
        agw::Class class_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        agw::Class class_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

