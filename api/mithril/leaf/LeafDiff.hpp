////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/leaf/Leaf.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Leaf::Diff {
        const Leaf              x;
        const Change            chg;
        const std::string       key;
        const Atom              atom;
        
        Changed<Image>          icon;
    };
}
