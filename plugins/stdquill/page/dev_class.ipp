////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    page_dev_class(WebHtml& h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
            
        Class::Info i  = info(c);
        h.title() << "Class (" << label(c) << ")";
        auto ta = h.table();
        h.kvrow("Name") << i.name;
        h.kvrow("Plural") << i.plural;
        h.kvrow("Brief") << i.brief;
        h.kvrow("Aliases") << join(aliases(c), ", ");
        h.kvrow("Prefixes") << join(prefixes(c), ", ");
        h.kvrow("Suffixes") << join(suffixes(c), ", ");
    }
    
    void    page_dev_class_tags(WebHtml& h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
            
        h.title() << "Class (" << label(c) << "): Tags";
        dev_table(h, tags(c, Sorted::YES));
        
    }

    
    YQ_INVOKE( 
        reg_webgroup({
            reg_webpage<page_dev_class>("/dev/class").argument("id", "Class ID").label("Info"),
            reg_webpage<page_dev_class_tags>("/dev/class/tags").argument("id", "Class ID").label("Tags")
        });
    )
    
}
