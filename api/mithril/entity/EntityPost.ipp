////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EntityPost.hpp"
#include <0/basic/TextUtils.hpp>
#include <mithril/entity/EntityArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/entity/Entity.hpp>

namespace yq::mithril::post {
    Entity entity(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("entity");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::entity_id(k);
        }
        return Entity();
    }
    
    Entity entity(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::entity_id(arg_string);
    }
    
    Entity entity(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::entity_id(arg_string);
    }
}
