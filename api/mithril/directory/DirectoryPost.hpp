////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <mithril/http/Post.hpp>

namespace yq::mithril {
    struct WebContext;
    struct Directory;
    
    namespace post {
        //! Finds root, by ID
        Directory directory(WebContext&, bool *detected=nullptr);
        Directory directory(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Directory directory(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
