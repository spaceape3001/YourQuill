////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_dev_tag(WebHtml& out)
    {
        Tag x   = tag(out);
        if(!x)
            throw HttpStatus::BadArgument;
        auto i = info(x);
        
        out.title() << "Tag \"" << i.name << "\" (" << i.key << ")";
        auto ta = out.table();
        out.kvrow("ID") << x.id;
        out.kvrow("Name") << i.name;
        out.kvrow("Key") << i.key;
        out.kvrow("Brief") << i.brief;
        out.kvrow("Document") << dev(i.doc);
        out.kvrow("Leaf"); //  << dev(leaf(x_tag));
        out.kvrow("Image");
    }
    
    YQ_INVOKE(
        reg_webpage<page_dev_tag>("/dev/tag")
            .argument("id", "Tag id (number)")
            .argument("key", "Tag key (string)")
            .argument("tag", "Tag key/identifier");
    )
}
