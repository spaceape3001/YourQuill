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
    struct Style;
    
    namespace post {
        //! Finds root_dir, by ID
        Style style(WebContext&, bool *detected=nullptr);
        Style style(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Style style(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
