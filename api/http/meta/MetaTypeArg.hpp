////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <vector>

namespace yq {
    struct WebContext;
    struct TypeInfo;
    
    namespace arg {
        //! Finds type_info, tries key first, then by ID
        const TypeInfo* type_info(std::string_view arg_string);
        const TypeInfo* type_info(const WebContext&, bool *detected=nullptr);
        const TypeInfo* type_info(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const TypeInfo* type_info(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds type_info by ID only
        const TypeInfo* type_info_id(std::string_view arg_string);
        const TypeInfo* type_info_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const TypeInfo* type_info_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds type_info by key only
        const TypeInfo* type_info_key(std::string_view arg_string);
        const TypeInfo* type_info_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const TypeInfo* type_info_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
