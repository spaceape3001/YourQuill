////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootPost.hpp"
#include <basic/TextUtils.hpp>
#include <aether/http/file/RootArg.hpp>
#include <aether/http/web/WebContext.hpp>

namespace yq {
    namespace post {
        const Root* root(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("root");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::root_id(k);
            }
            return nullptr;
        }
        
        const Root* root(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::root_id(arg_string);
        }
        
        const Root* root(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::root_id(arg_string);
        }
    }
}
