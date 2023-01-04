////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    p_leaf(WebHtml& h)
    {
        Leaf    x   = leaf(h.context());
        if(!x)
            throw HttpStatus::BadArgument;
        
        auto i      = info(x);
        auto dp     = merged(x);
            
        //  BIG FAT TODO!


        h.title() << "Leaf (" << i.key << ")";
        {
            auto t = h.table();
            h.kvrow("ID") << x.id;
            h.kvrow("Key") << i.key;
            h.kvrow("Title") << i.title;
            h.kvrow("Brief") << i.brief;
            h.kvrow("Document") << dev(i.doc);
            h.kvrow("Atom") << dev(i.atom);        
        }
        
        
    }
    
    void    p_leaf_random(WebContext& c)
    {
        //  Idea is to randomly redirect to a leaf
        
    }

    void    reg_leaf()
    {
        reg_webpage<p_leaf>("/leaf").argument("id", "Leaf ID");
        reg_webpage<p_leaf_random>("/leaf/random").argument("id", "Leaf ID");
    }
}
