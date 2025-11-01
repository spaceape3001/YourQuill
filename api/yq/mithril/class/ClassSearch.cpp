////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/category/Category.hpp>
#include <yq/mithril/category/CategoryArg.hpp>
#include <yq/mithril/class/ClassSearch.hpp>
#include <yq/mithril/class/ClassCDB.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    bool            search(WebSearcher<Class>&ws, category_k)
    {
        std::string s   = ws.ctx.find_query("category");
        if(s.empty())
            return false;
        
        Category x = arg::category(s);
        if(x){
            if(ws.first && ws.data.empty()){
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
    
    ClassVector     search(WebContext&ctx, class_k)
    {
        WebSearcher<Class>   ws(ctx);
        search(ws, CATEGORY);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_classes(Sorted::YES);
        return ws.data;
    }
}
