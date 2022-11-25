////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldPost.hpp"
#include <basic/TextUtils.hpp>
#include <http/agw/FieldArg.hpp>
#include <http/web/WebContext.hpp>
#include <kernel/agw/Field.hpp>

namespace yq {
    namespace post {
        agw::Field field(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("field");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::field_id(k);
            }
            return agw::Field();
        }
        
        agw::Field field(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::field_id(arg_string);
        }
        
        agw::Field field(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::field_id(arg_string);
        }
    }
}
