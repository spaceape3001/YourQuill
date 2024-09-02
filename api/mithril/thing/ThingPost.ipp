////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ThingPost.hpp"

#include <mithril/thing/ThingArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/thing/Thing.hpp>

namespace yq::mithril::post {
    Thing thing(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("thing");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::thing_id(k);
        }
        return Thing();
    }
    
    Thing thing(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::thing_id(arg_string);
    }
    
    Thing thing(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::thing_id(arg_string);
    }
}
