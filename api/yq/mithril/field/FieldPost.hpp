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
    struct Field;
    
    namespace post {
        //! Finds root_dir, by ID
        Field field(WebContext&, bool *detected=nullptr);
        Field field(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Field field(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
