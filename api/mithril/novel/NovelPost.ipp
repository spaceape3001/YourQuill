////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "NovelPost.hpp"

#include <mithril/novel/NovelArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/novel/Novel.hpp>

namespace yq::mithril::post {
    Novel novel(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("novel");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::novel_id(k);
        }
        return Novel();
    }
    
    Novel novel(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::novel_id(arg_string);
    }
    
    Novel novel(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::novel_id(arg_string);
    }
}
