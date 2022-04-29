////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/orgsys/category.hpp>

namespace yq {
    struct Category::Data {
        std::string     name;
        std::string     brief;
        std::string     notes;
        
        Data&   merge(const Data&, bool fOverride=false);
    };
}
