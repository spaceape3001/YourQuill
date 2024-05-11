////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/leaf/Leaf.hpp>

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
