////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryPost.hpp"
#include <0/basic/TextUtils.hpp>
#include <mithril/directory/DirectoryArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/directory/Directory.hpp>

namespace yq::mithril::post {
    Directory directory(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("directory");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::directory_id(k);
        }
        return Directory();
    }
    
    Directory directory(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::directory_id(arg_string);
    }
    
    Directory directory(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::directory_id(arg_string);
    }
}
