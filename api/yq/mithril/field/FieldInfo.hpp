////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/class/Class.hpp>
#include <yq/mithril/enum/Multiplicity.hpp>
#include <yq/mithril/enum/Restriction.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/category/Category.hpp>

namespace yq::mithril {
    struct Field::Info {
        Class           class_;
        Image           icon;
        Category        category;
        std::string     brief;
        std::string     key, pkey;
        std::string     name, plural;
        Id              expected;
        Restriction     restriction;
        Multiplicity    multiplicity;
        uint64_t        max_count   = 0;
        bool operator==(const Info&) const = default;
    };
}
