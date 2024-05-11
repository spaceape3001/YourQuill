
////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#error "Do NOT include (for now)"

#include "DoodleFile.hpp"
#include <mithril/bit/KeyValue.hpp>
#include <0/basic/ErrorDB.hpp>

namespace yq::errors {
    using no_doodle_in_file    = error_db::entry<"No doodle found in file">;
}
    
namespace yq::mithril {

    void    Doodle::File::reset() 
    {
        Data::reset();
    }

    std::error_code     Doodle::File::read(KVTree&&attrs, std::string_view) 
    {
        name    = attrs.value(kv::key({"%", "name"}));
        brief   = attrs.value("brief");
        return std::error_code(); 
    }
    
    std::error_code     Doodle::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("name", name);
        if(!brief.empty())
            attrs.set("brief", brief);
        return std::error_code(); 
    }
}

