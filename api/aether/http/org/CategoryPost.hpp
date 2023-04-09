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
    struct Category;
    
    namespace post {
        //! Finds root, by ID
        Category category(WebContext&, bool *detected=nullptr);
        Category category(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Category category(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
