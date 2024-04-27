////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

        void    dev_title(WebHtml& h, Document x, std::string_view extra=std::string_view())
        {
            auto t = h.title();
            h.title() << "Document (" << x.id << " &mdash; " << key(x) << ")";
            if(!extra.empty())
                h << ": " << extra;
        }
        
    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_document(WebHtml&h)
        {
            Document    doc = document(h);
            if(!doc)
                throw HttpStatus::BadArgument;
                
            auto i = info(doc);

            dev_title(h, doc);

            auto t = h.table();
            h.kvrow("ID") << doc.id;
            h.kvrow("Folder") << dev(i.folder);
            h.kvrow("Fragments") << fragments_count(doc);
            h.kvrow("Hidden") << i.hidden;
            
            if(is_similar(i.suffix, "class")){
                h.kvrow("As Class") << dev(class_(doc));
            } else if(is_similar(i.suffix, "field")){
                h.kvrow("As Field") << dev(field(doc));
            } else if(is_similar(i.suffix, "user")){
                h.kvrow("As User") << dev(user(doc));
            } else if(is_similar(i.suffix, "tag")){
                h.kvrow("As Tag") << dev(find_tag(doc));
            } else if(is_similar(i.suffix, "y")){
                h.kvrow("As Leaf") << dev(leaf(doc));
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
            dev_title(h, doc, "Attributes");
            dev_table(h, attributes(doc));
        }
        
        void    p_dev_document_fragments(WebHtml&h)
        {
            Document    doc = document(h);
            if(!doc)
                throw HttpStatus::BadArgument;
            dev_title(h, doc, "Fragments");
            dev_table(h, fragments(doc));
        }
        
        void    p_dev_document_roots(WebHtml&h)
        {
            Document    doc = document(h);
            if(!doc)
                throw HttpStatus::BadArgument;
            dev_title(h, doc, "Roots");
            dev_table(h, root_dirs(doc));
        }
        
        void    p_dev_documents(WebHtml& h)
        {
            h.title("Listing of Documents");
            dev_table(h, all_documents(Sorted::YES));
        }
    
    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_document_pages()
        {
            reg_webgroup({
                reg_webpage<p_dev_document>("/dev/document").argument("id", "Document ID").label("Info"),
                reg_webpage<p_dev_document_attributes>("/dev/document/attributes").argument("id", "Document ID").label("Attributes"),
                reg_webpage<p_dev_document_fragments>("/dev/document/fragments").argument("id", "Document ID").label("Fragments"),
                reg_webpage<p_dev_document_roots>("/dev/document/root_dirs").argument("id", "Document ID").label("Roots")
            });
            reg_webpage<p_dev_documents>("/dev/documents"); 
        }
}
