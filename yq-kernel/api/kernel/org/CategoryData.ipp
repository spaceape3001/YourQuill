////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryData.hpp"
#include <basic/TextUtils.hpp>

namespace yq {
    Category::Data&   Category::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        return *this;
    }
}

