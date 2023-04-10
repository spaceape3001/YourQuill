////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/org/Category.hpp>

namespace yq::mithril {
    struct Category::Data {
        std::string     name;
        std::string     brief;
        std::string     notes;
        
        Data&   merge(const Data&, bool fOverride=false);
    };
}
