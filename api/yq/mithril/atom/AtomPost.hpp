////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>

namespace yq::mithril {
    struct WebContext;
    
    namespace post {
        //! Finds atom, by ID
        Atom atom(WebContext&, bool *detected=nullptr);
        Atom atom(WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Atom atom(WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
