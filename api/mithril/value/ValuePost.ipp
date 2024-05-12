////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ValuePost.hpp"
#include <0/basic/TextUtils.hpp>
#include <mithril/value/ValueArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/value/Value.hpp>

namespace yq::mithril::post {
    Value value(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("value");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::value(k);
        }
        return Value();
    }
    
    Value value(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::value(arg_string);
    }
    
    Value value(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::value(arg_string);
    }
}
