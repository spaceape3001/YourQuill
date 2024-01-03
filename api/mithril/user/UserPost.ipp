////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UserPost.hpp"
#include <0/basic/TextUtils.hpp>
#include <mithril/user/UserArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/user/User.hpp>

namespace yq::mithril::post {
    User user(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("user");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::user_id(k);
        }
        return User();
    }
    
    User user(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::user_id(arg_string);
    }
    
    User user(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::user_id(arg_string);
    }
}
