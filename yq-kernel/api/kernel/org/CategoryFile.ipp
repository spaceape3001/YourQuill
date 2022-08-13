////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryFile.hpp"
#include <basic/TextUtils.hpp>
#include <kernel/bit/KeyValue.hpp>
#include <kernel/io/Strings.hpp>

namespace yq {
    Category::Data&   Category::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        return *this;
    }

////////////////////////////////////////////////////////////////////////////////

    void    Category::File::reset() 
    {
        *(Data*) this = Data{};
    }

    bool    Category::File::read(KVTree&&attrs, std::string_view fname) 
    { 
        name        = attrs.value(szName);
        brief       = attrs.value(szBrief);
        notes       = attrs.value(szNotes);
        return true; 
    }
        
    bool    Category::File::write(KVTree&attrs) const 
    { 
        if(!name.empty())
            attrs.set(szName, name);
        if(!brief.empty())
            attrs.set(szBrief, brief);
        if(!notes.empty())
            attrs.set(szNotes, notes);
        return true; 
    }
}

