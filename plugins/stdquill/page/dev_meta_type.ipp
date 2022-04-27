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
        h.kvrow("Aliases") << join(type->aliases(), ", ");
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
    
    void    page_dev_meta_type_properties(WebHtml& h)
    {
        const TypeInfo* type    = type_info(h);
        if(!type)
            throw HttpStatus::BadArgument;
        
        h.title() << "Type Info (" << type->name() << "): Properties";
        dev_table(h, type->properties());
    }
    
    YQ_INVOKE( 
        reg_webgroup({
            reg_webpage<page_dev_meta_type>("/dev/meta/type").label("Info"),
            reg_webpage<page_dev_meta_type_properties>("/dev/meta/type/properties").label("Properties")
        });
    )
    
}
