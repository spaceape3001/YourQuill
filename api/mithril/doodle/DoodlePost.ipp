////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DoodlePost.hpp"
#include <0/basic/TextUtils.hpp>
#include <mithril/doodle/DoodleArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/doodle/Doodle.hpp>

namespace yq::mithril::post {
    Doodle doodle(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("doodle");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::doodle_id(k);
        }
        return Doodle();
    }
    
    Doodle doodle(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::doodle_id(arg_string);
    }
    
    Doodle doodle(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::doodle_id(arg_string);
    }
}
