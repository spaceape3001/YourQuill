////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <http/Post.hpp>

namespace yq {
    struct WebContext;
    namespace agw {
        struct Field;
    }
    
    namespace post {
        //! Finds root, by ID
        agw::Field field(WebContext&, bool *detected=nullptr);
        agw::Field field(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        agw::Field field(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
