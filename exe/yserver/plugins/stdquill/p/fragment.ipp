////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/fragment/FragmentArg.hpp>
#include <mithril/fragment/FragmentJson.hpp>
#include <mithril/fragment/FragmentSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_fragment(WebContext& ctx)
        {
            Fragment    v   = arg::fragment(ctx, nullptr);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_fragment_key(WebContext& ctx)
        {
            Fragment    v   = arg::fragment(ctx, nullptr);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_fragment_path(WebContext& ctx)
        {
            Fragment    v   = arg::fragment(ctx, nullptr);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "path", cdb::path(v).string() }
            };
            return j;
        }

        json p_api_fragments(WebContext& ctx)
        {
            FragmentVector  ret = search(ctx, FRAGMENT);
            return json{
                { "fragments", json_(ret) }
            };
        }

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_fragment(WebHtml&h)
        {
            Fragment    x = fragment(h, nullptr);
            if(!x)
                throw HttpStatus::BadArgument;

            auto i = info(x);
            
            dev_title(h, x);

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
            h.kvrow("RootDir") << dev(i.root_dir);
            h.kvrow("Size") << i.size;
        }
        
        void    p_dev_fragment_content(WebHtml& h)
        {
            Fragment    x = fragment(h, nullptr);
            if(!x)
                throw HttpStatus::BadArgument;
                
            std::string     sfx = suffix(x).ext;

            dev_title(h, x, "Content");
            ContentType     ct = mimeTypeForExt( sfx);
            if(isImage(ct)){
                h << "<img src=\"/dev/fragment/image?" << h.context().url.query << "\" alt=\"Raw Image\">";
            } else if(isTextual(ct) || !ct){
                if(!is_similar(sfx, "user")){ // blank out user files (avoid passwords)
                    h << "<pre>";
                    std::string s = frag_string(x);
                    html_escape_write(h, s);
                    h << "</pre>";
                }
            } else if(isAudio(ct)){
                h.p() << "Audio File... TODO";
            } else if(isMovie(ct)){
                h.p() << "Movie File... TODO";
            } else {
                h.p() << "Unimplemented type: " << mimeType(ct);
            }
        }

        void    p_dev_fragment_image(WebContext& ctx)
        {
            Fragment x  = fragment(ctx, nullptr);
            if(!x)  
                throw HttpStatus::BadArgument;
        
            auto ext = suffix(x).ext;
            ctx.tx_content_type = mimeTypeForExt( ext );
            ctx.tx_content      = std::make_shared<ByteArray>(frag_bytes(x));
        }

        void    p_dev_fragments(WebHtml& h)
        {
            h.title("Listing of Fragments");
            dev_table(h, all_fragments(Sorted::YES));
        }
    
    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_fragment_pages()
        {
            reg_webpage<p_api_fragment>("/api/fragment").argument("ID", "Fragment ID");
            reg_webpage<p_api_fragment_key>("/api/fragment/key").argument("ID", "Fragment ID");
            reg_webpage<p_api_fragment_path>("/api/fragment/path").argument("ID", "Fragment ID");
            reg_webpage<p_api_fragments>("/api/fragments")
                .argument("directory", "Specify the owning directory (same as parent)")
                .argument("document", "Specify the document")
                .argument("folder", "Speccify the folder")
                .argument("parent", "Specify the parent directory (same as directory)")
                .argument("root", "Specify the root directory")
            ;

            reg_webgroup({
                reg_webpage<p_dev_fragment>("/dev/fragment").description("Developer info for a fragment").argument("id", "Fragment ID").label("Info"),
                reg_webpage<p_dev_fragment_content>("/dev/fragment/content").description("Content for fragment").argument("id", "Fragment ID").label("Content")
            });
            reg_webpage<p_dev_fragment_image>("/dev/fragment/image").argument("id", "ID for fragment"); 
            reg_webpage<p_dev_fragments>("/dev/fragments");
        }
}
