////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    p_dev_document(WebHtml&h)
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
        
        if(is_similar(i.suffix, "class")){
            h.kvrow("As Class") << dev(cdb::class_(doc));
        } else if(is_similar(i.suffix, "field")){
            h.kvrow("As Field") << dev(cdb::field(doc));
        } else if(is_similar(i.suffix, "user")){
            h.kvrow("As User") << dev(cdb::user(doc));
        } else if(is_similar(i.suffix, "tag")){
            h.kvrow("As Tag") << dev(cdb::find_tag(doc));
        } else if(is_similar(i.suffix, "y")){
            h.kvrow("As Leaf") << dev(cdb::leaf(doc));
        }
        
        h.kvrow("Key") << i.key;
        h.kvrow("Name") << i.name;
        h.kvrow("Removed") << i.removed;
        h.kvrow("SKey") << i.skey;
        h.kvrow("SKey B") << i.skeyb;
        h.kvrow("SKey C") << i.skeyc;
        h.kvrow("Suffix") << i.suffix;
    }
    
    void    p_dev_document_attributes(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << cdb::key(doc) << "): Attributes";
        dev_table(h, cdb::attributes(doc));
    }
    
    void    p_dev_document_fragments(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << cdb::key(doc) << "): Fragments";
        dev_table(h, cdb::fragments(doc));
    }
    
    void    p_dev_document_roots(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << cdb::key(doc) << "): Roots";
        dev_table(h, cdb::roots(doc));
    }
    
    void    p_dev_documents(WebHtml& h)
    {
        h.title("Listing of Documents");
        dev_table(h, all_documents(Sorted::YES));
    }
    
    void    reg_dev_document()
    {
        reg_webgroup({
            reg_webpage<p_dev_document>("/dev/document").argument("id", "Document ID").label("Info"),
            reg_webpage<p_dev_document_attributes>("/dev/document/attributes").argument("id", "Document ID").label("Attributes"),
            reg_webpage<p_dev_document_fragments>("/dev/document/fragments").argument("id", "Document ID").label("Fragments"),
            reg_webpage<p_dev_document_roots>("/dev/document/roots").argument("id", "Document ID").label("Roots")
        });
        reg_webpage<p_dev_documents>("/dev/documents"); 
    }
}
