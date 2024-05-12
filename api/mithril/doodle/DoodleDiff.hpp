////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/doodle/Doodle.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/util/Changed.hpp>


namespace yq::mithril {
    struct Doodle::Diff {
        const Doodle            x;
        const uint64_t          id;
        const Fragment          frag;
        const Document          doc;
        const Change            chg;
        
        Changed<Image>          icon;
        Changed<TagSet>         tags;
        Changed<std::string>    title;
    };
}
