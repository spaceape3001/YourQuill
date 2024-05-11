////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/novel/Novel.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Novel::Diff {
        const Novel              x;
        const uint64_t          id;
        const Change            chg;
        const std::string       key;
        const Document          doc;
        
        Changed<Image>          icon;
        Changed<std::string>    title;
        Changed<TagSet>         tags;
    };
}
