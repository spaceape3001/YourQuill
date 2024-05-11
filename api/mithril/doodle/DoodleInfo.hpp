////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/doodle/Doodle.hpp>

namespace yq::mithril {
    struct Doodle::Info {
        Doodle          doodle;       
        Document        doc;
        std::string     key;
        //std::string     brief;
        std::string     name;
        Image           icon;
        bool operator==(const Info&) const = default;
    };
}
