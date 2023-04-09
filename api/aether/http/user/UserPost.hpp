////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <aether/http/Post.hpp>

namespace yq {
    struct WebContext;
    struct User;
    
    namespace post {
        //! Finds root, by ID
        User user(WebContext&, bool *detected=nullptr);
        User user(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        User user(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
