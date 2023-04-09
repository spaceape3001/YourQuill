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
    struct Leaf;
    
    namespace post {
        //! Finds root, by ID
        Leaf leaf(WebContext&, bool *detected=nullptr);
        Leaf leaf(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Leaf leaf(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
