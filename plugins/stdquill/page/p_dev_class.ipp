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
        h.kvrow("Aliases") << join(def_aliases(c), ", ");
        h.kvrow("Prefixes") << join(def_prefixes(c), ", ");
        h.kvrow("Suffixes") << join(def_suffixes(c), ", ");
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

        h.title() << "Class (" << label(c) << "): Def Fields";
        dev_table(h, def_fields(c, Sorted::YES));
    }

    void    page_dev_classes(WebHtml&h)
    {
        h.title() << "All Classes";
        dev_table(h, all_classes(Sorted::YES));
    }
    
    void    page_dev_class_atoms(WebHtml&h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(c) << "): Atoms";
        dev_table(h, atoms(c, Sorted::YES));
    }
    
    
    void reg_dev_class()
    {
        reg_webgroup({
            reg_webpage<page_dev_class>("/dev/class").argument("id", "Class ID").label("Info"),
            reg_webpage<page_dev_class_tags>("/dev/class/tags").argument("id", "Class ID").label("Tags"),
            reg_webpage<page_dev_class_def_fields>("/dev/class/def_fields").argument("id", "Class ID").label("Def Fields"),
            reg_webpage<page_dev_class_atoms>("/dev/class/atoms").argument("id", "Class ID").label("Atoms")
        });
        reg_webpage<page_dev_classes>("/dev/classes");
    }
}
