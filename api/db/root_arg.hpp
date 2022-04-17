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
    
    namespace arg {
        //! Finds root, tries key first, then by ID
        const Root* root(std::string_view arg_string);
        const Root* root(const WebContext&);
        const Root* root(const WebHtml&);
        const Root* root(const WebContext&, std::string_view arg_name);
        const Root* root(const WebHtml&, std::string_view arg_name);
        const Root* root(const WebContext&, std::initializer_list<std::string_view> arg_names);
        const Root* root(const WebHtml&, std::initializer_list<std::string_view> arg_names);

        //! Finds root by ID only
        const Root* root_id(std::string_view arg_string);
        const Root* root_id(const WebContext&, std::string_view arg_name);
        const Root* root_id(const WebHtml&, std::string_view arg_name);
        const Root* root_id(const WebContext&, std::initializer_list<std::string_view> arg_names);
        const Root* root_id(const WebHtml&, std::initializer_list<std::string_view> arg_names);
        
        //! Finds root by key only
        const Root* root_key(std::string_view arg_string);
        const Root* root_key(const WebContext&, std::string_view arg_name);
        const Root* root_key(const WebHtml&, std::string_view arg_name);
        const Root* root_key(const WebContext&, std::initializer_list<std::string_view> arg_names);
        const Root* root_key(const WebHtml&, std::initializer_list<std::string_view> arg_names);
    }
}
