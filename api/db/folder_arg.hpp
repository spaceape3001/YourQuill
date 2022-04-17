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
    class WebHtml;
    struct Root;
    struct Folder;
    
    namespace arg {

        //! Finds folder, tries key first, then by ID
        Folder folder(std::string_view arg_string);
        Folder folder(const WebContext&);
        Folder folder(const WebHtml&);
        Folder folder(const WebContext&, std::string_view arg_name);
        Folder folder(const WebHtml&, std::string_view arg_name);
        Folder folder(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Folder folder(const WebHtml&, std::initializer_list<std::string_view> arg_names);

        //! Finds folder by ID only
        Folder folder_id(std::string_view arg_string);
        Folder folder_id(const WebContext&, std::string_view arg_name);
        Folder folder_id(const WebHtml&, std::string_view arg_name);
        Folder folder_id(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Folder folder_id(const WebHtml&, std::initializer_list<std::string_view> arg_names);
        
        //! Finds folder by KEY only
        Folder folder_key(std::string_view arg_string);
        Folder folder_key(const WebContext&, std::string_view arg_name);
        Folder folder_key(const WebHtml&, std::string_view arg_name);
        Folder folder_key(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Folder folder_key(const WebHtml&, std::initializer_list<std::string_view> arg_names);
    }
}
