////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/kernel/atom/Atom.hpp>
#include <aether/kernel/image/Image.hpp>
#include <aether/kernel/leaf/Leaf.hpp>

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
