////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/atom/Atom.hpp>
#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/leaf/Leaf.hpp>

namespace yq::mithril {
    struct Leaf::Info {
        Atom            atom;       
        Document        doc;
        std::string     key;
        std::string     title;
        std::string     brief;
        std::string     abbr;
        Image           icon;
        bool operator==(const Info&) const = default;
    };
}
