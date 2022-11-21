////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FolderArg.hpp"

#include <basic/TextUtils.hpp>

#include <http/Post.hpp>
#include <http/WebContext.hpp>

#include <kernel/FolderCDB.hpp>

namespace yq {
    namespace arg {
        Folder folder(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Folder{};
                
            Folder t   = cdb::folder( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_folder(i))
                return Folder{i};
            return Folder{};
        }
        
        Folder folder(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            
            if(detected)
                *detected   = false;
            
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return folder_id(k);
            }
            
            k       = ctx.find_query("key");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return folder_key(k);
            }
            
            k       = ctx.find_query("folder");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return folder(k);
            }
            return Folder{};
        }
        
        Folder folder(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return folder(arg_string);
        }
        
        Folder folder(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return folder(arg_string);
        }
        
        Folder folder_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_folder(i))
                return Folder{i};
            return Folder{};
        }

        Folder folder_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return folder_id(arg_string);
        }
        
        
        Folder folder_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return folder_id(arg_string);
        }
        
        
        Folder folder_key(std::string_view arg_string)
        {
            return cdb::folder(trimmed(arg_string));
        }

        Folder folder_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return folder_key(arg_string);
        }
        
        Folder folder_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return folder_key(arg_string);
        }
    }

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
