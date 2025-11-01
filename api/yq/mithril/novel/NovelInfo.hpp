////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/novel/Novel.hpp>

namespace yq::mithril {
    struct Novel::Info {
        std::string     title;
        Image           icon;
        bool operator==(const Info&) const = default;
    };
}
