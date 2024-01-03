////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomPost.hpp>
#include <0/basic/TextUtils.hpp>
#include <mithril/atom/AtomArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/atom/Atom.hpp>

namespace yq::mithril::post {
    Atom atom(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("atom");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::atom_id(k);
        }
        return Atom();
    }
    
    Atom atom(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::atom_id(arg_string);
    }
    
    Atom atom(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::atom_id(arg_string);
    }
}
