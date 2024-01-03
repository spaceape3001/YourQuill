////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderPost.hpp"
#include <0/basic/TextUtils.hpp>
#include <mithril/folder/FolderArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/folder/Folder.hpp>

namespace yq::mithril::post {
    Folder folder(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("folder");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::folder_id(k);
        }
        return Folder();
    }
    
    Folder folder(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::folder_id(arg_string);
    }
    
    Folder folder(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::folder_id(arg_string);
    }
}
