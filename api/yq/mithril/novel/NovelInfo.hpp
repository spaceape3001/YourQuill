////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/image/Image.hpp>
#include <mithril/novel/Novel.hpp>

namespace yq::mithril {
    struct Novel::Info {
        std::string     title;
        Image           icon;
        bool operator==(const Info&) const = default;
    };
}
