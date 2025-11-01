////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/atom/Atom.hpp>

namespace yq::mithril {
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

    namespace arg {
        //! Finds atom_property, by ID
        Atom::Property atom_property(std::string_view arg_string);
        Atom::Property atom_property(const WebContext&, bool *detected=nullptr);
        Atom::Property atom_property(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Atom::Property atom_property(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds atom_property by ID
        Atom::Property atom_property_id(std::string_view arg_string);
        Atom::Property atom_property_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Atom::Property atom_property_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

