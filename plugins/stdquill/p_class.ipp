////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void page_class(WebHtml& h)
    {
        Class x = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        h.title() << "Info for class " << cdb::label(x);
        h << "TODO!";
    }
    
    void page_class_atoms(WebHtml&h)
    {
        Class x = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << cdb::plural(x);
        h << "TODO!";
    }

    void    reg_class()
    {
        reg_webgroup({
            reg_webpage<page_class>("/class").argument("ID", "Class ID").label("Overview"),
            reg_webpage<page_class_atoms>("/class/atoms").argument("ID", "Class ID").label("Atoms")
        });
    }
}
