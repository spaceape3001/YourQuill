////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <http/Post.hpp>

namespace yq {
    struct WebContext;
    struct Fragment;
    
    namespace post {
        //! Finds root, by ID
        Fragment fragment(WebContext&, bool *detected=nullptr);
        Fragment fragment(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Fragment fragment(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
