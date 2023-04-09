////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <aether/http/Post.hpp>

namespace yq {
    struct WebContext;
    struct Folder;
    
    namespace post {
        //! Finds root, by ID
        Folder folder(WebContext&, bool *detected=nullptr);
        Folder folder(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Folder folder(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
