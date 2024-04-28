
////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DrafterFile.hpp"
#include <mithril/bit/KeyValue.hpp>
#include <0/basic/ErrorDB.hpp>

namespace yq::errors {
    using no_drafter_in_file    = error_db::entry<"No drafter found in file">;
}
    
namespace yq::mithril {

    void    Drafter::File::reset() 
    {
        Data::reset();
    }

    std::error_code     Drafter::File::read(KVTree&&attrs, std::string_view) 
    {
        name    = attrs.value(kv::key({"%", "name"}));
        brief   = attrs.value("brief");
        return std::error_code(); 
    }
    
    std::error_code     Drafter::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("name", name);
        if(!brief.empty())
            attrs.set("brief", brief);
        return std::error_code(); 
    }
}

