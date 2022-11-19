////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/agw/Atom.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/leaf/Leaf.hpp>

namespace yq {
    namespace agw {
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
}
