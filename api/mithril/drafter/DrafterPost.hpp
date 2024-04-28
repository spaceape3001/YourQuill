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
    struct Drafter;
    
    namespace post {
        //! Finds root_dir, by ID
        Drafter drafter(WebContext&, bool *detected=nullptr);
        Drafter drafter(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Drafter drafter(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
