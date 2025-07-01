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
    struct TypeMeta;
}

namespace yq::mithril {
    struct WebContext;
    
    namespace arg {
        //! Finds type_info, tries key first, then by ID
        const TypeMeta* type_info(std::string_view arg_string);
        const TypeMeta* type_info(const WebContext&, bool *detected=nullptr);
        const TypeMeta* type_info(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const TypeMeta* type_info(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds type_info by ID only
        const TypeMeta* type_info_id(std::string_view arg_string);
        const TypeMeta* type_info_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const TypeMeta* type_info_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds type_info by key only
        const TypeMeta* type_info_key(std::string_view arg_string);
        const TypeMeta* type_info_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const TypeMeta* type_info_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
