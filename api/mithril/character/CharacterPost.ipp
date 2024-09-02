////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CharacterPost.hpp"

#include <mithril/character/CharacterArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/character/Character.hpp>

namespace yq::mithril::post {
    Character character(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("character");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::character_id(k);
        }
        return Character();
    }
    
    Character character(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::character_id(arg_string);
    }
    
    Character character(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::character_id(arg_string);
    }
}
