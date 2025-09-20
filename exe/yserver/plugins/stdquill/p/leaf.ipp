////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/leaf/LeafJson.hpp>
#include <mithril/leaf/LeafSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_leaf(WebContext& ctx)
        {
            Leaf    v   = arg::leaf(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_leaf_key(WebContext& ctx)
        {
            Leaf    v   = arg::leaf(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_leafs(WebContext& ctx)
        {
            LeafVector  ret = search(ctx, LEAF);
            return json{
                { "leafs", json_(ret) }
            };
        }

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
        
        std::string_view title_of(const Context& ctx)
        {
            if(!ctx.title.empty())
                return ctx.title;
            if(ctx.root_dir){
                if(!ctx.root_dir->name.empty())
                    return ctx.root_dir->name;
                if(!ctx.root_dir->key.empty())
                    return ctx.root_dir->key;
            }
            return std::string_view();
        }
        
        void    leaf_page(WebHtml& h, Leaf x)
        {
            
            auto i      = info(x);
            auto dp     = merged(x);
                
            //  BIG FAT TODO!


            if(!i.title.empty()){
                h.title(i.title);
            } else 
                h.title(i.key);
            {
                h << "<table class=\"attrs\" align=\"left\">";
                if(i.icon){
                    h << "<tr><td colspan=\"2\"><IMG src=\"/image?id=" << i.icon.id << "\" class=\"leaf\" /></td></tr>";
                    h << "<tr><td colspan=\"2\"><hr /></td></tr>";
                }
            
                h.kvrow("ID") << x.id;
                h.kvrow("Key") << i.key;
                h.kvrow("Title") << i.title;
                h.kvrow("Brief") << i.brief;
                h.kvrow("Document") << dev(i.doc);
                h.kvrow("Atom") << dev(i.atom); 
                
                h << "</table>";  
            }
            
            for(auto& c : dp -> context){
                h.h2(title_of(c));
                
                switch(c.format){
                case Format::Markdown:
                    h << markdown(c.data);
                    break;
                case Format::ASCII:
                case Format::Plain:
                default:
                    h.pre(c.data);
                    break;
                }
            }
        }

        void    p_wiki_view(WebHtml& h)
        {
            Leaf    x   = leaf(h.context());
            if(!x)
                throw HttpStatus::BadArgument;
            leaf_page(h, x);
        }
        
        void    p_wiki_glob(WebHtml& h)
        {
            Leaf    x   = cdb::leaf(h.context().truncated_path);
            if(!x)
                throw HttpStatus::BadArgument;
            leaf_page(h, x);
        }
        
        
        void    p_wiki_browse(WebHtml& h)
        {
            Folder  x    = folder(h);
            if(!x)
                x   = top_folder();
            
            XEntry::Details  detail;
            detail.query.folders    = true;
            detail.query.leafs      = true;
            detail.link.folder      = "/wiki/browse?folder=";
            detail.link.leaf        = "/wiki/view?leaf=";
            detail.icon_size   = h.context().session.icon_size;
            
            std::vector<XEntry>  entries = XEntry::query(x, detail);

            if(x == top_folder()){
                h.title() << "/ : Wiki Browser";
            } else {
                h.title() << cdb::key(x) << " : Wiki Browser";
            }

            h << "<div id=\"explorer\">\n";
            {
                auto tac = h.table();
                html::columns(h, entries,  
                    [&](const XEntry& ee){
                        if(ee.type == XEntry::None)
                            return;

                        auto al = ee.write_link(h, detail);
                        ee.write_thumbnail(h, detail);
                    }, 
                    [&](const XEntry& ee){
                        if(ee.type == XEntry::None)
                            return;
                        
                        auto isLink = ee.write_link(h, detail);
                        switch(ee.type){
                        case XEntry::IsFolder:
                            html_escape_write(h, ee.skey);
                            return;
                        case XEntry::IsLeaf:
                            if(!ee.title.empty()){
                                html_escape_write(h, ee.title);
                            } else {
                                html_escape_write(h, ee.skey);
                            }
                        default:
                            return;
                        }
                    }
                );
            }
            h << "</div>\n";
        }
        
        void    p_wiki_random(WebContext& c)
        {
            Leaf x  = random_pick(cdb::all_leafs(Sorted::NO));
            std::string s   = "/wiki/view?key="s + cdb::key(x);
            throw redirect::see_other(s);
        }


    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------


        void p_dev_leaf(WebHtml&h)
        {
            Leaf    x   = leaf(h);
            if(!x)
                throw HttpStatus::BadArgument;
            
            auto i = info(x);

            dev_title(h, x);
            auto t = h.table();
            h.kvrow("ID") << x.id;
            h.kvrow("Key") << i.key;
            h.kvrow("Title") << i.title;
            h.kvrow("Brief") << i.brief;
            h.kvrow("Document") << dev(i.doc);
            h.kvrow("Atom") << dev(i.atom);
       }
        
        //  we'll do more (later)

        void    p_dev_leafs(WebHtml&h)
        {
            h.title("All Leafs");
            dev_table(h, all_leafs(Sorted::YES));
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_leaf_pages()
        {
            reg_webpage<p_api_leaf>("/api/leaf").argument("ID", "Leaf ID");
            reg_webpage<p_api_leaf_key>("/api/leaf/key").argument("ID", "Leaf ID");
            reg_webpage<p_api_leafs>("/api/leafs");

            reg_webpage<p_wiki_view>("/leaf").argument("id", "Leaf ID").argument("key", "Key");
            reg_webpage<p_wiki_view>("/wiki/view").argument("id", "Leaf ID").argument("key", "Key");
            reg_webpage<p_wiki_browse>("/wiki/browse").argument("folder", "Folder ID");
            reg_webpage<p_wiki_random>("/wiki/random").argument("id", "Leaf ID").argument("key", "Key");
            reg_webpage<p_wiki_glob>("/w/**");

            reg_webgroup({
                reg_webpage<p_dev_leaf>("/dev/leaf").argument("id", "Leaf ID").label("Info")
            });
            reg_webpage<p_dev_leafs>("/dev/leafs");

            reg_webtemplate("/leafs", wksp::shared("www/leafs.ht"sv));
        }
}
