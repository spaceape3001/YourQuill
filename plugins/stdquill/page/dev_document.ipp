////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    page_dev_document(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
            
        auto i = cdb::info(doc);

        h.title() << "Document (" << i.key << ")";

        auto t = h.table();
        h.kvrow("ID") << doc.id;
        h.kvrow("Folder") << dev(i.folder);
        h.kvrow("Fragments") << cdb::fragments_count(doc);
        h.kvrow("Hidden") << i.hidden;
        
        if(is_similar(i.suffix, "cls")){
            //Class c = cdb::class_(doc);
            //if(!c)
                //yInfo() << "Undefined class! " << i.key << " " << doc.id;
            //if(!cdb::exists_class(doc.id))
                //yInfo() << "Exists class is FALSE!";
            h.kvrow("Is class") << dev(cdb::class_(doc));
        } else if(is_similar(i.suffix, "fld")){
            h.kvrow("Is field") << dev(cdb::field(doc));
        } else if(is_similar(i.suffix, "usr")){
            h.kvrow("Is user") << dev(cdb::user(doc));
        } else if(is_similar(i.suffix, "tag")){
            h.kvrow("Is tag") << dev(cdb::tag(doc));
        } else if(is_similar(i.suffix, "y")){
            h.kvrow("Is leaf") << dev(cdb::leaf(doc));
        }
        
        h.kvrow("Key") << i.key;
        h.kvrow("Name") << i.name;
        h.kvrow("Removed") << i.removed;
        h.kvrow("SKey") << i.skey;
        h.kvrow("SKey B") << i.skeyb;
        h.kvrow("SKey C") << i.skeyc;
        h.kvrow("Suffix") << i.suffix;
    }
    
    void    page_dev_document_attributes(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << cdb::key(doc) << "): Attributes";
        dev_table(h, cdb::attributes(doc));
    }
    
    void    page_dev_document_fragments(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << cdb::key(doc) << "): Fragments";
        dev_table(h, cdb::fragments(doc));
    }
    
    void    page_dev_document_roots(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << cdb::key(doc) << "): Roots";
        dev_table(h, cdb::roots(doc));
    }
    
    YQ_INVOKE( 
        reg_webgroup({
            reg_webpage<page_dev_document>("/dev/document").argument("id", "Document ID").label("Info"),
            reg_webpage<page_dev_document_attributes>("/dev/document/attributes").argument("id", "Document ID").label("Attributes"),
            reg_webpage<page_dev_document_fragments>("/dev/document/fragments").argument("id", "Document ID").label("Fragments"),
            reg_webpage<page_dev_document_roots>("/dev/document/roots").argument("id", "Document ID").label("Roots")
        });
    )
}
