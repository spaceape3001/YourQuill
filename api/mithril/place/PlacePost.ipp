////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlacePost.hpp"

#include <mithril/place/PlaceArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/place/Place.hpp>

namespace yq::mithril::post {
    Place place(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("place");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::place_id(k);
        }
        return Place();
    }
    
    Place place(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::place_id(arg_string);
    }
    
    Place place(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::place_id(arg_string);
    }
}
