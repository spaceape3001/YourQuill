////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "StylePost.hpp"

#include <mithril/style/StyleArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/style/Style.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    Style style(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("style");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::style_id(k);
        }
        return Style();
    }
    
    Style style(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::style_id(arg_string);
    }
    
    Style style(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::style_id(arg_string);
    }
}
