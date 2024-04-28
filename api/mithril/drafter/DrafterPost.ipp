////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DrafterPost.hpp"
#include <0/basic/TextUtils.hpp>
#include <mithril/drafter/DrafterArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/drafter/Drafter.hpp>

namespace yq::mithril::post {
    Drafter drafter(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("drafter");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::drafter_id(k);
        }
        return Drafter();
    }
    
    Drafter drafter(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::drafter_id(arg_string);
    }
    
    Drafter drafter(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::drafter_id(arg_string);
    }
}
