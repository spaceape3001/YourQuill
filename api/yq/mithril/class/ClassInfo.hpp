////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/class/Class.hpp>
#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/category/Category.hpp>

namespace yq::mithril {
    struct Class::Info {
        std::string key;
        Category    category;
        std::string binding;
        //Graph       deps;
        Document    doc;
        Image       icon;
        std::string name;
        std::string plural;
        std::string brief;
        bool        edge    = false;

        bool operator==(const Info&) const = default;
    };
}
