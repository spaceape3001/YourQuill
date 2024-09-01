////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "StyleData.hpp"
//#include <0/basic/CollectionUtils.hpp>
#include <0/basic/TextUtils.hpp>

namespace yq::mithril {
    Style::Data&      Style::Data::merge(const Style::Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        return *this;
    }

    void    Style::Data::reset() 
    {
        name.clear();
        brief.clear();
    }
}
