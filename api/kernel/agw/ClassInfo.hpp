////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/agw/Class.hpp>
#include <kernel/file/Document.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/org/Category.hpp>

namespace yq {
    namespace agw {
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
}
