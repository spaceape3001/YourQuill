////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UserPost.hpp"
#include <basic/TextUtils.hpp>
#include <http/user/UserArg.hpp>
#include <http/web/WebContext.hpp>
#include <kernel/user/User.hpp>

namespace yq {
    namespace post {
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
}
