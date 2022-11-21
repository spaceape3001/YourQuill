////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/Category.hpp>
#include <kernel/Class.hpp>
#include <kernel/Document.hpp>
#include <kernel/Image.hpp>

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
