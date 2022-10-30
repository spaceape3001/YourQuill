////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void p_dev_leaf(WebHtml&h)
    {
        Leaf    x   = leaf(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        auto i = cdb::info(x);
            
        h.title() << "Leaf (" << i.key << ")";
        auto t = h.table();
        h.kvrow("ID") << x.id;
        h.kvrow("Key") << i.key;
        h.kvrow("Title") << i.title;
        h.kvrow("Brief") << i.brief;
        h.kvrow("Document") << dev(i.doc);
        h.kvrow("Atom") << dev(i.atom);
   }
    
    //  we'll do more (later)

    void    p_dev_leafs(WebHtml&h)
    {
        h.title("All Leafs");
        dev_table(h, all_leafs(Sorted::YES));
    }

    void    reg_dev_leaf()
    {
        reg_webgroup({
            reg_webpage<p_dev_leaf>("/dev/leaf").argument("id", "Leaf ID").label("Info")
        });
        reg_webpage<p_dev_leafs>("/dev/leafs");
    }
}
