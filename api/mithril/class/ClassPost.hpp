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
    struct Class;
    
    namespace post {
        //! Finds root, by ID
        Class class_(WebContext&, bool *detected=nullptr);
        Class class_(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Class class_(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
