////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/Document.hpp>
#include <kernel/Leaf.hpp>
#include <kernel/Image.hpp>
#include <kernel/Tag.hpp>

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
