////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/novel/Novel.hpp>
#include <yq/mithril/enum/Change.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Novel::Diff {
        const Novel             x;
        const uint64_t          id;
        const Fragment          frag;
        const Document          doc;
        const Change            chg;
        
        Changed<Image>          icon;
        Changed<TagSet>         tags;
        Changed<std::string>    title;
    };
}
