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
    struct RootDir;
    
    namespace post {
        //! Finds root_dir, by ID
        const RootDir* root_dir(WebContext&, bool *detected=nullptr);
        const RootDir* root_dir(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const RootDir* root_dir(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
