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
        dev_table(h, obj->bases(true));
    }

    void page_dev_meta_object_all_derived(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): All Derived";
        dev_table(h, obj->deriveds(true));
    }

    void page_dev_meta_object_derived(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): Derived";
        dev_table(h, obj->deriveds(false));
    }

    YQ_INVOKE( 
        reg_webgroup({
            reg_webpage<page_dev_meta_object>("/dev/meta/object").label("Info"),
            reg_webpage<page_dev_meta_object_derived>("/dev/meta/object/derived").label("Derived"),
            reg_webpage<page_dev_meta_object_all_base>("/dev/meta/object/all_base").label("All Base"),
            reg_webpage<page_dev_meta_object_all_derived>("/dev/meta/object/all_derived").label("All Derived")
        });
    )
}
