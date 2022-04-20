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
            throw HttpStatus::BadArgument;

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
    
    void    page_dev_fragment_content(WebHtml& h)
    {
        Fragment    x = fragment(h, nullptr);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Fragment (" << path(x) << "): Content";
        ContentType     ct = mimeTypeForExt( suffix(x));
        if(isImage(ct)){
            h << "<img src=\"/dev/fragment/image?" << h.context().url.query << "\" alt=\"Raw Image\">";
        } else if(isTextual(ct) || !ct){
            h << "<pre>";
            std::string s = frag_string(x);
            html_escape_write(h, s);
            h << "</pre>";
        } else if(isAudio(ct)){
            h.p() << "Audio File... TODO";
        } else if(isMovie(ct)){
            h.p() << "Movie File... TODO";
        } else {
            h.p() << "Unimplemented type: " << mimeType(ct);
        }
    }

    YQ_INVOKE( 
        reg_webgroup({
            reg_webpage<page_dev_fragment>("/dev/fragment").description("Developer info for a fragment").argument("id", "Fragment ID").label("Info"),
            reg_webpage<page_dev_fragment_content>("/dev/fragment/content").description("Content for fragment").argument("id", "Fragment ID").label("Content")
        });
    )
    
}
