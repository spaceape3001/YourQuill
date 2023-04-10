////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <mithril/html/Post.hpp>

namespace yq::mithril {
    struct WebContext;
    struct Leaf;
    
    namespace post {
        //! Finds root, by ID
        Leaf leaf(WebContext&, bool *detected=nullptr);
        Leaf leaf(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Leaf leaf(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
