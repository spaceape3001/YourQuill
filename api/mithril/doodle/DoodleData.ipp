////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#error "Do NOT include (for now)"

#include "DoodleData.hpp"
#include <0/basic/CollectionUtils.hpp>
#include <0/basic/TextUtils.hpp>

namespace yq::mithril {
    Doodle::Data&      Doodle::Data::merge(const Doodle::Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        return *this;
    }

    void    Doodle::Data::reset() 
    {
        name.clear();
        brief.clear();
    }
}
