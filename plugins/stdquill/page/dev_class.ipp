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
        h.kvrow("ID") << c.id;
        h.kvrow("Name") << i.name;
        h.kvrow("Plural") << i.plural;
        h.kvrow("Category") << dev(i.category);
        h.kvrow("Brief") << i.brief;
        h.kvrow("Aliases") << join(aliases(c), ", ");
        h.kvrow("Prefixes") << join(prefixes(c), ", ");
        h.kvrow("Suffixes") << join(suffixes(c), ", ");
        h.kvrow("Binding") << i.binding;
    }
    
    void    page_dev_class_tags(WebHtml& h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
            
        h.title() << "Class (" << label(c) << "): Tags";
        dev_table(h, tags(c, Sorted::YES));
        
    }
    
    void    page_dev_class_def_fields(WebHtml&h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
        
        std::vector<Field>  flist   = def_fields(c);
        flist += def_fields(Class());
        std::sort(flist.begin(), flist.end(), Field::less_label);
            
        h.title() << "Class (" << label(c) << "): Def Fields";
        dev_table(h, flist);
    }

    
    YQ_INVOKE( 
        reg_webgroup({
            reg_webpage<page_dev_class>("/dev/class").argument("id", "Class ID").label("Info"),
            reg_webpage<page_dev_class_tags>("/dev/class/tags").argument("id", "Class ID").label("Tags"),
            reg_webpage<page_dev_class_def_fields>("/dev/class/def_fields").argument("id", "Class ID").label("Def Fields")
        });
    )
    
}
