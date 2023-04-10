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
    struct Atom;
    
    namespace post {
        //! Finds root, by ID
        Atom atom(WebContext&, bool *detected=nullptr);
        Atom atom(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Atom atom(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
