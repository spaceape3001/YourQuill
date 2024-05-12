////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/category/Category.hpp>
#include <mithril/category/CategoryArg.hpp>
#include <mithril/class/ClassSearch.hpp>
#include <mithril/class/ClassCDB.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Class>&ws, category_t)
    {
        std::string s   = ws.ctx.find_query("category");
        if(s.empty())
            return false;
        
        Category x = arg::category(s);
        if(x){
            if(ws.first && !ws.data.empty()){
                ws.data     = cdb::all_classes(x, Sorted::YES);
            } else {
                std::erase_if(ws.data, 
                    [x](Class c) -> bool 
                    { 
                        return cdb::category(c) != x; 
                    }
                );
            }
        } else
            ws.data.clear();
        ws.first    = true;
        return true;
    }
    
    ClassVector     search(WebContext&ctx, class_t)
    {
        WebSearcher<Class>   ws(ctx);
        search(ws, CATEGORY);
        if(ws.first)
            ws.data  = cdb::all_classes(Sorted::YES);
        return ws.data;
    }
}
