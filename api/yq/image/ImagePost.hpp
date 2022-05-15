////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <yq/db/Post.hpp>

namespace yq {
    struct WebContext;
    struct Image;
    
    namespace post {
        //! Finds root, by ID
        Image image(WebContext&, bool *detected=nullptr);
        Image image(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Image image(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
