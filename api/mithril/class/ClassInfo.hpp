////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/atom/Class.hpp>
#include <mithril/kernel/file/Document.hpp>
#include <mithril/kernel/image/Image.hpp>
#include <mithril/kernel/org/Category.hpp>

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
