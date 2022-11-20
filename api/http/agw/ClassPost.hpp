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
        struct Class;
    }
    
    namespace post {
        //! Finds root, by ID
        agw::Class class_(WebContext&, bool *detected=nullptr);
        agw::Class class_(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        agw::Class class_(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
