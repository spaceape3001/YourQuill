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
    struct Folder;
    
    namespace post {
        //! Finds root_dir, by ID
        Folder folder(WebContext&, bool *detected=nullptr);
        Folder folder(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Folder folder(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
