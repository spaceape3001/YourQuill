////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void page_dev_atom(WebHtml& h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        
        Atom::Info  i   = info(a);
        
        h.title() << "Atom (" << label(a) << ")";

        auto ta = h.table();
        h.kvrow("ID") << a.id;
        h.kvrow("Name") << i.name;
        h.kvrow("Key") << i.key;
        h.kvrow("Abbreviation") << i.abbr;
        h.kvrow("Brief") << i.brief;
        //h.kvrow("Leaf") << dev(i.leaf);
    }
    
    void page_dev_atom_classes(WebHtml&h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Atom (" << label(a) << "): Classes";
        dev_table(h, classes(a, Sorted::YES));
    }
    
    void page_dev_atom_documents(WebHtml& h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Atom (" << label(a) << "): Documents";
        dev_table(h, documents(a, Sorted::YES));
    }
    
    void page_dev_atom_tags(WebHtml& h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Atom (" << label(a) << "): Tags";
        dev_table(h, tags(a, Sorted::YES));
    }

    void page_dev_atoms(WebHtml&h)
    {
        h.title() << "All Atoms";
        dev_table(h, all_atoms(Sorted::YES));
    }
    
    void reg_dev_atom()
    {
        reg_webgroup({
            reg_webpage<page_dev_atom>("/dev/atom").argument("id", "Atom ID").label("Info"),
            reg_webpage<page_dev_atom_classes>("/dev/atom/classes").argument("id", "Atom ID").label("Classes"),
            reg_webpage<page_dev_atom_documents>("/dev/atom/documents").argument("id", "Atom ID").label("Docs"),
            reg_webpage<page_dev_atom_tags>("/dev/atom/tags").argument("id", "Atom ID").label("Tags")
        });
        reg_webpage<page_dev_atoms>("/dev/atoms");
    }
}
