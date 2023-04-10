////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagData.hpp"
#include <basic/TextUtils.hpp>

namespace yq::mithril {

    Tag::Data& Tag::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(leaf, b.leaf, fOverride);
        set_if_empty(name, b.name, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        return *this;
    }

    void    Tag::Data::reset() 
    {
        name.clear();
        leaf.clear();
        brief.clear();
        notes.clear();
    }
}
