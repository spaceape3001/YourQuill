////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void page_dev_leaf(WebHtml&h)
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
    
    YQ_INVOKE( 
        reg_webpage<page_dev_leaf>("/dev/leaf").argument("id", "Leaf ID").label("Info");
    )
}
