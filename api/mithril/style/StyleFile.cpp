
////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "StyleFile.hpp"
#include <yq/keyv/KeyValue.hpp>
#include <yq/core/ErrorDB.hpp>

namespace yq::errors {
    using no_style_in_file    = error_db::entry<"No style found in file">;
}
    
namespace yq::mithril {

    void    Style::File::reset() 
    {
        Data::reset();
    }

    std::error_code     Style::File::read(KVTree&&attrs, std::string_view) 
    {
        name    = attrs.value(kv::key({"%", "name"}));
        brief   = attrs.value("brief");
        return std::error_code(); 
    }
    
    std::error_code     Style::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("name", name);
        if(!brief.empty())
            attrs.set("brief", brief);
        return std::error_code(); 
    }
}

