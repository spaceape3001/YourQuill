////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/file/Document.hpp>
#include <mithril/kernel/leaf/Leaf.hpp>
#include <mithril/kernel/image/Image.hpp>
#include <mithril/kernel/org/Tag.hpp>

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
