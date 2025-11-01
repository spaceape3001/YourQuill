////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/doodle/Doodle.hpp>

namespace yq::mithril {
    struct Doodle::Info {
        //Doodle          doodle;       
        //Document        doc;
        //std::string     key;
        //std::string     brief;
        std::string     name;
        Image           icon;
        bool operator==(const Info&) const = default;
    };
}
