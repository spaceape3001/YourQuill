////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/atom/Atom.hpp>
#include <mithril/kernel/image/Image.hpp>
#include <mithril/kernel/leaf/Leaf.hpp>

namespace yq {
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
