////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DrafterData.hpp"
#include <0/basic/CollectionUtils.hpp>
#include <0/basic/TextUtils.hpp>

namespace yq::mithril {
    Drafter::Data&      Drafter::Data::merge(const Drafter::Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        return *this;
    }

    void    Drafter::Data::reset() 
    {
        name.clear();
        brief.clear();
    }
}
