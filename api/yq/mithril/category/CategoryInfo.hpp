////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/category/Category.hpp>

namespace yq::mithril {
    struct Category::Info {
        std::string key;
        Document    doc;
        Image       icon;
        std::string name;
        std::string brief;

        bool operator==(const Info&) const = default;
    };
}
