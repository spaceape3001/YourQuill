////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <yq/mithril/html/Post.hpp>

namespace yq::mithril {
    struct WebContext;
    struct Category;
    
    namespace post {
        //! Finds root_dir, by ID
        Category category(WebContext&, bool *detected=nullptr);
        Category category(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Category category(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
