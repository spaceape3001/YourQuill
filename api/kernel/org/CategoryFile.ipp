////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryFile.hpp"
#include <kernel/bit/KeyValue.hpp>
#include <kernel/io/Strings.hpp>

namespace yq {
    void    Category::File::reset() 
    {
        *(Data*) this = Data{};
    }

    std::error_code    Category::File::read(KVTree&&attrs, std::string_view) 
    { 
        name        = attrs.value(szName);
        brief       = attrs.value(szBrief);
        notes       = attrs.value(szNotes);
        return std::error_code(); 
    }
        
    std::error_code    Category::File::write(KVTree&attrs) const 
    { 
        if(!name.empty())
            attrs.set(szName, name);
        if(!brief.empty())
            attrs.set(szBrief, brief);
        if(!notes.empty())
            attrs.set(szNotes, notes);
        return std::error_code(); 
    }
}

