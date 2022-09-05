////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void page_dev_meta_object(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << ")";
        auto ta = h.table();
        
        h.kvrow("ID") << obj -> id();
        h.kvrow("Name") << obj -> name();
        h.kvrow("Base") << dev(obj->base());
        h.kvrow("Description") << obj->description();
        h.kvrow("Size") << obj->size();
        h.kvrow("Abstract?") << obj->is_abstract();
    }
    
    void page_dev_meta_object_all_base(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): All Bases";
        dev_table(h, obj->bases(true).all);
    }

    void page_dev_meta_object_all_derived(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): All Derived";
        dev_table(h, obj->deriveds(true).all);
    }

    void page_dev_meta_object_all_properties(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): All Properties";
        dev_table(h, obj->properties(true).all);
    }

    void page_dev_meta_object_derived(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): Derived";
        dev_table(h, obj->deriveds(false).all);
    }

    void page_dev_meta_object_properties(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): Properties";
        dev_table(h, obj->properties(false).all);
    }
    
    void    page_dev_meta_objects(WebHtml&h)
    {
        h.title() << "All Meta Objects";
        dev_table(h, ObjectInfo::all());
    }

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
    
    void page_dev_meta_types(WebHtml&h)
    {
        h.title() << "All Type Infos";
        dev_table(h, TypeInfo::all());
    }

    void    reg_dev_meta()
    {
        reg_webgroup({
            reg_webpage<page_dev_meta_object>("/dev/meta/object").label("Info"),
            reg_webpage<page_dev_meta_object_derived>("/dev/meta/object/derived").label("Derived"),
            reg_webpage<page_dev_meta_object_properties>("/dev/meta/object/properties").label("Properties"),
            reg_webpage<page_dev_meta_object_all_base>("/dev/meta/object/all_base").label("All Base"),
            reg_webpage<page_dev_meta_object_all_derived>("/dev/meta/object/all_derived").label("All Derived"),
            reg_webpage<page_dev_meta_object_all_properties>("/dev/meta/object/all_properties").label("All Properties")
        });
        reg_webpage<page_dev_meta_objects>("/dev/meta/objects"); 
        reg_webgroup({
            reg_webpage<page_dev_meta_type>("/dev/meta/type").label("Info"),
            reg_webpage<page_dev_meta_type_properties>("/dev/meta/type/properties").label("Properties")
        });
        reg_webpage<page_dev_meta_types>("/dev/meta/types");
    }
}
