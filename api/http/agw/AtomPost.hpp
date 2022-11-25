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
        struct Atom;
    }
    
    namespace post {
        //! Finds root, by ID
        agw::Atom atom(WebContext&, bool *detected=nullptr);
        agw::Atom atom(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        agw::Atom atom(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
