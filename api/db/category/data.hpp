////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "struct.hpp"

namespace yq {
    struct Category::Data {
        std::string     name;
        std::string     brief;
        std::string     notes;
        
        Data&   merge(const Data&, bool fOverride=false);
    };
}
