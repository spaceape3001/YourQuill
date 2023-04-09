////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagPost.hpp"
#include <basic/TextUtils.hpp>
#include <aether/http/org/TagArg.hpp>
#include <aether/http/web/WebContext.hpp>
#include <aether/kernel/org/Tag.hpp>

namespace yq {
    namespace post {
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
}
