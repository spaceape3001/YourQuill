////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/drafter/Drafter.hpp>

namespace yq::mithril {
    struct Drafter::Info {
        Drafter           drafter;       
        Document        doc;
        std::string     key;
        std::string     brief;
        std::string     name;
        Image           icon;
        bool operator==(const Info&) const = default;
    };
}
