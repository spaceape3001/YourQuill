////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Random.hpp>

namespace {
    std::string_view title_of(const Context& ctx)
    {
        if(!ctx.title.empty())
            return ctx.title;
        if(ctx.root){
            if(!ctx.root->name.empty())
                return ctx.root->name;
            if(!ctx.root->key.empty())
                return ctx.root->key;
        }
        return std::string_view();
    }

    void    p_leaf(WebHtml& h)
    {
        Leaf    x   = leaf(h.context());
        if(!x)
            throw HttpStatus::BadArgument;
        
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
    
    void    p_leaf_random(WebContext& c)
    {
        Leaf x  = random_pick(cdb::all_leafs(Sorted::NO));
        std::string s   = "/leaf?key="s + cdb::key(x);
        throw redirect::see_other(s);
    }

    void    reg_leaf()
    {
        reg_webpage<p_leaf>("/leaf").argument("id", "Leaf ID");
        reg_webpage<p_leaf_random>("/leaf/random").argument("id", "Leaf ID");
    }
}
