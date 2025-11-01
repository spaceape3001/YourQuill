////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/enum/Change.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/leaf/Leaf.hpp>
#include <yq/mithril/util/Changed.hpp>

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
