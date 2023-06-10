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
    struct Image;
    
    namespace post {
        //! Finds root_dir, by ID
        Image image(WebContext&, bool *detected=nullptr);
        Image image(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Image image(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
