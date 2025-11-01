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
        const uint64_t          id;
        const Change            chg;
        const std::string       key;
        const Document          doc;
        const Atom              atom;
        
        Changed<Image>          icon;
        Changed<std::string>    title;
        Changed<std::string>    brief;
        Changed<std::string>    abbr;
        Changed<TagSet>         tags;
    };
}
