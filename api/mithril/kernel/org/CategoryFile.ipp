////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryFile.hpp"
#include <mithril/kernel/bit/KeyValue.hpp>
#include <mithril/kernel/io/Strings.hpp>

namespace yq::mithril {        
    void    Category::File::reset() 
    {
        *(Data*) this = Data{};
    }

    std::error_code    Category::File::read(KVTree&&attrs, std::string_view) 
    { 
        name        = attrs.value(kv::key({"%", "name"}));
        brief       = attrs.value("brief");
        notes       = attrs.value("notes");
        return std::error_code(); 
    }
        
    std::error_code    Category::File::write(KVTree&attrs) const 
    { 
        if(!name.empty())
            attrs.set("name", name);
        if(!brief.empty())
            attrs.set("brief", brief);
        if(!notes.empty())
            attrs.set("notes", notes);
        return std::error_code(); 
    }
}
