////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <mithril/http/Post.hpp>

namespace yq {
    struct WebContext;
    struct Root;
    
    namespace post {
        //! Finds root, by ID
        const Root* root(WebContext&, bool *detected=nullptr);
        const Root* root(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const Root* root(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
