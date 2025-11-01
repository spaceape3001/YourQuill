////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/atom/Atom.hpp>
#include <yq/mithril/attribute/Attribute.hpp>
#include <yq/mithril/field/Field.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/leaf/Leaf.hpp>

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
