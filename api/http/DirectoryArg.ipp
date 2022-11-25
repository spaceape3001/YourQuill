////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryArg.hpp"

#include <basic/TextUtils.hpp>
#include <http/FolderArg.hpp>
#include <http/RootArg.hpp>
#include <http/WebContext.hpp>
#include <kernel/DirectoryCDB.hpp>
#include <kernel/FolderCDB.hpp>

namespace yq {
    namespace arg {

        Directory directory(std::string_view  arg_string, const Root* rt)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Directory{};
                
            Folder      folder = folder_key(arg_string);
            if(folder)
                return cdb::first_directory(folder, rt);

            return directory_id(arg_string);
        }
        
        Directory directory(const WebContext&ctx, const Root* rt, bool *detected)
        {
            std::string    k;
            
            if(detected)
                *detected   = false;
            
            k    = ctx.find_query("id");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return directory_id(k);
            }
            
            k   = ctx.find_query("root");
            if(!k.empty())
                rt  = root(k);

            k       = ctx.find_query("key");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return directory_key(k, rt);
            }
            
            k       = ctx.find_query("directory");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return directory(k, rt);
            }
            return Directory{};
        }
        
        Directory directory(const WebContext&ctx, std::string_view arg_name, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return directory(arg_string, rt);
        }
        
        Directory directory(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return directory(arg_string, rt);
        }
        
        Directory directory_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_directory(i))
                return Directory{i};
            return Directory{};
        }

        Directory directory_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return directory_id(arg_string);
        }
        
        Directory directory_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return directory_id(arg_string);
        }
        
        
        Directory directory_key(std::string_view arg_string, const Root* rt)
        {
            Folder  folder  = folder_key(arg_string);
            if(!folder)
                return Directory();
            return cdb::first_directory(folder, rt);
        }
        
        Directory directory_key(const WebContext&ctx, std::string_view arg_name, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return directory_key(arg_string, rt);
        }
        
        Directory directory_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return directory_key(arg_string, rt);
        }
    }
}
