////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/kernel/file/Document.hpp>
#include <aether/kernel/leaf/Leaf.hpp>
#include <aether/kernel/image/Image.hpp>
#include <aether/kernel/org/Tag.hpp>

namespace yq {
    
    struct Tag::Info {
        std::string     brief;
        Document        doc;
        std::string     key;
        Image           icon;
        Leaf            leaf;
        std::string     name;
        bool operator==(const Info&) const = default;
    };
}
