////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq {
    struct WebContext;
    struct ObjectInfo;
    
    namespace arg {
        //! Finds object_info, tries key first, then by ID
        const ObjectInfo* object_info(std::string_view arg_string);
        const ObjectInfo* object_info(const WebContext&, bool *detected=nullptr);
        const ObjectInfo* object_info(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const ObjectInfo* object_info(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds object_info by ID only
        const ObjectInfo* object_info_id(std::string_view arg_string);
        const ObjectInfo* object_info_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const ObjectInfo* object_info_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds object_info by key only
        const ObjectInfo* object_info_key(std::string_view arg_string);
        const ObjectInfo* object_info_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const ObjectInfo* object_info_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
