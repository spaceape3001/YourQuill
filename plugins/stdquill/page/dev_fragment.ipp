////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    page_dev_fragment(WebHtml&h)
    {
        Fragment    x = fragment(h, nullptr);
        if(!x)
            throw HttpStatus::NotAcceptable;

        auto i = info(x);
        h.title() << "Fragment (" << i.path.string() << ")";

        auto _t = h.table();
        h.kvrow("ID") << x.id;
        h.kvrow("Document") << dev(i.document);
        h.kvrow("Directory") << dev(i.directory);
        h.kvrow("Folder") << dev(i.folder);
        h.kvrow("Hidden") << i.hidden;
        h.kvrow("Modified") << i.modified << " ns.";
        h.kvrow("Name") << i.name;
        h.kvrow("Path") << i.path;
        h.kvrow("Removed") << i.removed;
        h.kvrow("Rescan") << i.rescan;
        h.kvrow("Root") << dev(i.root);
        h.kvrow("Size") << i.size;
    }

    YQ_INVOKE( reg_webpage<page_dev_fragment>("/dev/fragment").description("Developer info for a fragment").argument("id", "Fragment ID"); )
    
}
