////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagPost.hpp"
#include <basic/TextUtils.hpp>
#include <mithril/http/org/TagArg.hpp>
#include <mithril/http/web/WebContext.hpp>
#include <mithril/kernel/org/Tag.hpp>

namespace yq::mithril::post {
    Tag tag(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("tag");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::tag_id(k);
        }
        return Tag();
    }
    
    Tag tag(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::tag_id(arg_string);
    }
    
    Tag tag(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::tag_id(arg_string);
    }
}
