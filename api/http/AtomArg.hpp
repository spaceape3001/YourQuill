////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq {
    struct Atom;

    struct WebContext;
    
    namespace arg {
        //! Finds atom, tries key first, then by ID
        Atom atom(std::string_view arg_string);
        Atom atom(const WebContext&, bool *detected=nullptr);
        Atom atom(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Atom atom(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds atom by ID only
        Atom atom_id(std::string_view arg_string);
        Atom atom_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Atom atom_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds atom by KEY only
        Atom atom_key(std::string_view arg_string);
        Atom atom_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Atom atom_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

