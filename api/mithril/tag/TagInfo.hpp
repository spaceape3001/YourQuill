////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>
#include <mithril/leaf/Leaf.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/tag/Tag.hpp>

namespace yq::mithril {
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
