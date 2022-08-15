////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderPost.hpp"
#include <basic/TextUtils.hpp>
#include <http/file/FolderArg.hpp>
#include <http/web/WebContext.hpp>
#include <kernel/file/Folder.hpp>

namespace yq {
    namespace post {
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
}
