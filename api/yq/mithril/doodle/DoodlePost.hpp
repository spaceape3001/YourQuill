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
    struct Doodle;
    
    namespace post {
        //! Finds root_dir, by ID
        Doodle doodle(WebContext&, bool *detected=nullptr);
        Doodle doodle(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Doodle doodle(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
