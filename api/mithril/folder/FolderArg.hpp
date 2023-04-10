////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq::mithril {
    struct WebContext;
    struct Root;
    struct Folder;
    
    namespace arg {

        //! Finds folder, tries key first, then by ID
        Folder folder(std::string_view arg_string);
        Folder folder(const WebContext&, bool *detected=nullptr);
        Folder folder(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Folder folder(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds folder by ID only
        Folder folder_id(std::string_view arg_string);
        Folder folder_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Folder folder_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds folder by KEY only
        Folder folder_key(std::string_view arg_string);
        Folder folder_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Folder folder_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
