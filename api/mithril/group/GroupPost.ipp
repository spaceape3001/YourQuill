////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GroupPost.hpp"

#include <mithril/group/GroupArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/group/Group.hpp>

namespace yq::mithril::post {
    Group group(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("group");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::group_id(k);
        }
        return Group();
    }
    
    Group group(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::group_id(arg_string);
    }
    
    Group group(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::group_id(arg_string);
    }
}
