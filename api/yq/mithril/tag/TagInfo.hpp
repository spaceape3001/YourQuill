////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/leaf/Leaf.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/tag/Tag.hpp>

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
