////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "GraphvizPost.hpp"

#include <mithril/graphviz/GraphvizArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/graphviz/Graphviz.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    Graphviz graphviz(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("graphviz");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::graphviz(k);
        }
        return Graphviz();
    }
    
    Graphviz graphviz(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::graphviz(arg_string);
    }
    
    Graphviz graphviz(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::graphviz(arg_string);
    }
}
