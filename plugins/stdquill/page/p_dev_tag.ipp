////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    dev_title(WebHtml& h, Tag x, std::string_view extra=std::string_view())
    {
        auto t = h.title();
        auto i = nki(x);
        h << "Tag \"" << i.name << "\" (" << i.key << ")";
        if(!extra.empty())
            h << ": " << extra;
    }

    void    page_dev_tag(WebHtml& h)
    {
        Tag x   = tag(h);
        if(!x)
            throw HttpStatus::BadArgument;
        auto i = info(x);
        
        dev_title(h, x);
        auto ta = h.table();
        h.kvrow("ID") << x.id;
        h.kvrow("Name") << i.name;
        h.kvrow("Key") << i.key;
        h.kvrow("Brief") << i.brief;
        h.kvrow("Document") << dev(i.doc);
        h.kvrow("Leaf"); //  << dev(leaf(x_tag));
        h.kvrow("Image");
    }
    
    void    page_dev_tag_classes(WebHtml& h)
    {
        Tag x   = tag(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        dev_title(h, x, "Classes");
        dev_table(h, classes(x));
    }

    void    page_dev_tags(WebHtml& out)
    {
        out.title("All Tags");
        dev_table(out, all_tags());
    }

    void    reg_dev_tag()
    {
        reg_webgroup({
            reg_webpage<page_dev_tag>("/dev/tag")
                .argument("id", "Tag id (number)")
                .argument("key", "Tag key (string)")
                .argument("tag", "Tag key/identifier").label("Info"),
            reg_webpage<page_dev_tag_classes>("/dev/tag/classes").argument("id", "Tag id (number)").label("Classes")
        });
        reg_webpage<page_dev_tags>("/dev/tags");
    }
}
