////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/web/WebContext.hpp>

namespace yq::mithril {
    template <typename T>
    struct WebSearcher {
        WebContext&         ctx;
        std::vector<T>      data;
        bool                first  = true;
        WebSearcher(WebContext& c) : ctx(c) {}
    };
    
    template <typename T, typename LookupFN, typename AllFN, typename EraseFN>
    bool search(WebSearcher<T>& ws, std::string_view k, LookupFN lookup, AllFN all, EraseFN erase)
    {
        std::string s   = ws.ctx.find_query(k);
        if(s.empty())
            return false;
            
        auto   x   = lookup(s);
        if(x){
            if(ws.first && ws.data.empty()){
                ws.data     = all(x);
            } else {
                std::erase_if(ws.data, [x,erase](T y) -> bool {
                    return erase(x,y);
                });
            }
        } else
            ws.data.clear();
        ws.first    = true;
        return true;
    }
    
}
