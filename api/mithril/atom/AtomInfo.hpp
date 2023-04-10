////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>
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
        bool operator==(const Info&) const = default;
    };
}
