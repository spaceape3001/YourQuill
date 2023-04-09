////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryPost.hpp"
#include <basic/TextUtils.hpp>
#include <mithril/http/org/CategoryArg.hpp>
#include <mithril/http/web/WebContext.hpp>
#include <mithril/kernel/org/Category.hpp>

namespace yq {
    namespace post {
        Category category(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("category");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::category_id(k);
            }
            return Category();
        }
        
        Category category(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::category_id(arg_string);
        }
        
        Category category(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::category_id(arg_string);
        }
    }
}
