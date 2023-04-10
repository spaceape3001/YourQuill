////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassPost.hpp"
#include <basic/TextUtils.hpp>
#include <mithril/http/atom/ClassArg.hpp>
#include <mithril/http/web/WebContext.hpp>
#include <mithril/kernel/atom/Class.hpp>

namespace yq::mithril::post {

    Class class_(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("class");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::class_id(k);
        }
        return Class();
    }
    
    Class class_(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::class_id(arg_string);
    }
    
    Class class_(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::class_id(arg_string);
    }
}
