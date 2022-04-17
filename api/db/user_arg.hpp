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
    struct User;
    
    namespace arg {
        //! Finds user, tries key first, then by ID
        User user(std::string_view arg_string);
        User user(const WebContext&);
        User user(const WebHtml&);
        User user(const WebContext&, std::string_view arg_name);
        User user(const WebHtml&, std::string_view arg_name);
        User user(const WebContext&, std::initializer_list<std::string_view> arg_names);
        User user(const WebHtml&, std::initializer_list<std::string_view> arg_names);

        //! Finds user by ID only
        User user_id(std::string_view arg_string);
        User user_id(const WebContext&, std::string_view arg_name);
        User user_id(const WebHtml&, std::string_view arg_name);
        User user_id(const WebContext&, std::initializer_list<std::string_view> arg_names);
        User user_id(const WebHtml&, std::initializer_list<std::string_view> arg_names);
        
        //! Finds user by KEY only
        User user_key(std::string_view arg_string);
        User user_key(const WebContext&, std::string_view arg_name);
        User user_key(const WebHtml&, std::string_view arg_name);
        User user_key(const WebContext&, std::initializer_list<std::string_view> arg_names);
        User user_key(const WebHtml&, std::initializer_list<std::string_view> arg_names);
    }
}

