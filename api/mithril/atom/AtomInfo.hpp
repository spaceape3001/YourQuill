////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>
#include <mithril/attribute/Attribute.hpp>
#include <mithril/field/Field.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/leaf/Leaf.hpp>

namespace yq::mithril {
    struct Atom::Info {
        std::string     abbr;
        std::string     brief;
        std::string     key;
        Leaf            leaf; 
        std::string     name;
        Image           icon;
        Atom            parent;
        bool operator==(const Info&) const = default;
    };

    struct Atom::PropertyInfo {
        Atom            atom;
        Attribute       attr;
        Field           field;
        Atom            source;
        Atom            target;
        Atom            child;
        bool operator==(const PropertyInfo&) const = default;
    };
}
