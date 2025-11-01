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
    struct Fragment;
    
    namespace post {
        //! Finds root_dir, by ID
        Fragment fragment(WebContext&, bool *detected=nullptr);
        Fragment fragment(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Fragment fragment(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
