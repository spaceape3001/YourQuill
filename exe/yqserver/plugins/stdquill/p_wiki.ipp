////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/Random.hpp>

namespace {


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

    void    reg_wiki()
    {
        reg_webpage<p_wiki_view>("/leaf").argument("id", "Leaf ID").argument("key", "Key");
        reg_webpage<p_wiki_view>("/wiki/view").argument("id", "Leaf ID").argument("key", "Key");
        reg_webpage<p_wiki_browse>("/wiki/browse").argument("folder", "Folder ID");
        reg_webpage<p_wiki_random>("/wiki/random").argument("id", "Leaf ID").argument("key", "Key");
        reg_webpage<p_wiki_glob>("/w/**");
    }
}
