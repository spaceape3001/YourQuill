////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassPost.hpp"
#include <basic/TextUtils.hpp>
#include <http/agw/ClassArg.hpp>
#include <http/web/WebContext.hpp>
#include <kernel/agw/Class.hpp>

namespace yq {
    namespace post {

        agw::Class class_(WebContext&ctx, bool *detected)
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
            return agw::Class();
        }
        
        agw::Class class_(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::class_id(arg_string);
        }
        
        agw::Class class_(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::class_id(arg_string);
        }
    }
}
