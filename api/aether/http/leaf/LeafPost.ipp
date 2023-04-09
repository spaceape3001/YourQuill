////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafPost.hpp"
#include <basic/TextUtils.hpp>
#include <aether/http/leaf/LeafArg.hpp>
#include <aether/http/web/WebContext.hpp>
#include <aether/kernel/leaf/Leaf.hpp>

namespace yq {
    namespace post {
        Leaf leaf(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("leaf");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::leaf_id(k);
            }
            return Leaf();
        }
        
        Leaf leaf(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::leaf_id(arg_string);
        }
        
        Leaf leaf(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::leaf_id(arg_string);
        }
    }
}
