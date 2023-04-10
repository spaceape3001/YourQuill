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
    struct Tag;
    
    namespace post {
        //! Finds root, by ID
        Tag tag(WebContext&, bool *detected=nullptr);
        Tag tag(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Tag tag(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
