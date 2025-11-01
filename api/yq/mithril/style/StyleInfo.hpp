////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/style/Style.hpp>

namespace yq::mithril {
    struct Style::Info {
        Style           style;       
        Document        doc;
        std::string     key;
        std::string     brief;
        std::string     name;
        Image           icon;
        bool operator==(const Info&) const = default;
    };
}
