////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/kernel/atom/Class.hpp>
#include <aether/kernel/file/Document.hpp>
#include <aether/kernel/image/Image.hpp>
#include <aether/kernel/org/Category.hpp>

namespace yq {
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
