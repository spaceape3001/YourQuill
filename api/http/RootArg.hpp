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
    struct Root;
    
    namespace arg {
        //! Finds root, tries key first, then by ID
        const Root* root(std::string_view arg_string);
        const Root* root(const WebContext&, bool *detected=nullptr);
        const Root* root(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const Root* root(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds root by ID only
        const Root* root_id(std::string_view arg_string);
        const Root* root_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const Root* root_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds root by key only
        const Root* root_key(std::string_view arg_string);
        const Root* root_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const Root* root_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }

    namespace post {
        //! Finds root, by ID
        const Root* root(WebContext&, bool *detected=nullptr);
        const Root* root(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const Root* root(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
