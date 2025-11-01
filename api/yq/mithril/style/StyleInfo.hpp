////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/style/Style.hpp>

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
