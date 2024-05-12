////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>
#include <mithril/atom/AtomArg.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/class/ClassArg.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithril/tag/TagArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    bool        search(WebSearcher<Atom>&ws, class_t)
    {
        std::string s   = ws.ctx.find_query("class");
        if(s.empty())
            return false;
        
        Class x = arg::class_(s);
        if(x){
            if(ws.first && !ws.data.empty()){
                ws.data     = cdb::all_atoms(x, Sorted::YES);
            } else {
                std::erase_if(ws.data, 
                    [x](Atom a) -> bool 
                    { 
                        return !cdb::is(a, x); 
                    }
                );
            }
        } else
            ws.data.clear();
        ws.first    = true;
        return true;
    }
    
    bool        search(WebSearcher<Atom>&ws, tag_t)
    {
        std::string s   = ws.ctx.find_query("tag");
        if(s.empty())
            return false;
        
        Tag x = arg::tag(s);
        if(x){
            if(ws.first && !ws.data.empty()){
                ws.data     = cdb::all_atoms(x, Sorted::YES);
            } else {
                std::erase_if(ws.data, 
                    [x](Atom a) -> bool 
                    { 
                        return !cdb::is(a, x); 
                    }
                );
            }
        } else
            ws.data.clear();
        ws.first    = true;
        return true;
    }

    AtomVector  search(WebContext&ctx, atom_t)
    {
        WebSearcher<Atom>   ws(ctx);
        search(ws, TAG);
        search(ws, CLASS);
        if(ws.first)
            ws.data  = cdb::all_atoms(Sorted::YES);
        return ws.data;
    }
}
