////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <kernel/SQ.hpp>

namespace {

    void    p_dev_class(WebHtml& h)
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
        //h.kvrow("Aliases") << join(def_aliases(c), ", ");
        //h.kvrow("Prefixes") << join(def_prefixes(c), ", ");
        //h.kvrow("Suffixes") << join(def_suffixes(c), ", ");
        h.kvrow("Binding") << i.binding;
    }
    
    void    p_dev_class_atoms(WebHtml&h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(c) << "): Atoms";
        dev_table(h, cdb::all_atoms(c, Sorted::YES));
    }
    
    void    p_dev_class_base(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        h.title() << "Class (" << label(x) << "): Base";
        dev_table(h, cdb::base_classes_ranked(x, Sorted::YES), "Hops");
    }
    
    void    p_dev_class_derived(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(x) << "): Derived";
        dev_table(h, cdb::derived_classes_ranked(x, Sorted::YES), "Hops");
    }

    void    p_dev_class_fields(WebHtml&h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(c) << "): Def Fields";
        dev_table(h, fields(c, Sorted::YES));
    }

    void    p_dev_class_inbound(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(x) << "): Inbound";
        dev_table(h, cdb::inbound_classes_ranked(x, Sorted::YES), "Hops");
    }

    void    p_dev_class_outbound(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(x) << "): Outbound";
        dev_table(h, cdb::outbound_classes_ranked(x, Sorted::YES), "Hops");
    }

    void    p_dev_class_sources(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(x) << "): Sources";
        dev_table(h, cdb::source_classes_ranked(x, Sorted::YES), "Hops");
    }

    void    p_dev_class_tags(WebHtml& h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
            
        h.title() << "Class (" << label(c) << "): Tags";
        dev_table(h, tags(c, Sorted::YES));
        
    }
    void    p_dev_class_targets(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(x) << "): Sources";
        dev_table(h, cdb::source_classes_ranked(x, Sorted::YES), "Hops");
    }

    
    /*
    void    p_dev_class_def_fields(WebHtml&h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(c) << "): Def Fields";
        dev_table(h, def_fields(c, Sorted::YES));
    }
    */

    void    p_dev_classes(WebHtml&h)
    {
        h.title() << "All Classes";
        dev_table(h, all_classes(Sorted::YES));
    }
    
    
    void reg_dev_class()
    {
        reg_webgroup({
            reg_webpage<p_dev_class>("/dev/class").argument("id", "Class ID").label("Info"),
            reg_webpage<p_dev_class_base>("/dev/class/base").argument("id", "Class ID").label("Base"),
            reg_webpage<p_dev_class_derived>("/dev/class/derived").argument("id", "Class ID").label("Derived"),
            reg_webpage<p_dev_class_sources>("/dev/class/sources").argument("id", "Class ID").label("Sources"),
            reg_webpage<p_dev_class_targets>("/dev/class/targets").argument("id", "Class ID").label("Targets"),
            reg_webpage<p_dev_class_inbound>("/dev/class/inbound").argument("id", "Class ID").label("Inbound"),
            reg_webpage<p_dev_class_outbound>("/dev/class/outbound").argument("id", "Class ID").label("Outbound"),
            reg_webpage<p_dev_class_tags>("/dev/class/tags").argument("id", "Class ID").label("Tags"),
            reg_webpage<p_dev_class_fields>("/dev/class/fields").argument("id", "Class ID").label("Fields"),
            //reg_webpage<p_dev_class_def_fields>("/dev/class/def_fields").argument("id", "Class ID").label("Def Fields"),
            reg_webpage<p_dev_class_atoms>("/dev/class/atoms").argument("id", "Class ID").label("Atoms")
        });
        reg_webpage<p_dev_classes>("/dev/classes");
    }
}
