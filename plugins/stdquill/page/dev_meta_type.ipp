////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/type/Any.hpp>

namespace {

    void    page_dev_meta_type(WebHtml& h)
    {
        const TypeInfo* type    = type_info(h);
        if(!type)
            throw HttpStatus::BadArgument;
        
        h.title() << "Type Info (" << type->name() << ")";
        auto ta = h.table();
        h.kvrow("ID") << type -> id();
        h.kvrow("Name") << type->name();
        h.kvrow("Size") << type->size();
        
        
        h.kvrow("Small?") << type->is_small();
        h.kvrow("Parsable?") << type->can_parse();
        h.kvrow("Writable?") << type->can_write();
        h.kvrow("Printable?") << type->can_print();
        
        Any     def(type);
        h.kvrow("Default") << "'" << def.printable() << "'";

        h.kvrow("Properties") << type->property_count();
        h.kvrow("Methods") << type->method_count();
    }
    
    YQ_INVOKE( 
        reg_webpage<page_dev_meta_type>("/dev/meta/type");
        
        //  going to web-group this with properties (later)
    )
    
}
