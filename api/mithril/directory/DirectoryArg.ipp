////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DirectoryArg.hpp"

#include <basic/TextUtils.hpp>
#include <mithril/folder/FolderArg.hpp>
#include <mithril/root/RootArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/directory/DirectoryCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>

namespace yq::mithril::arg {

    Directory directory(std::string_view  arg_string, const RootDir* rt)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Directory();
            
        Folder      folder = folder_key(arg_string);
        if(folder)
            return cdb::first_directory(folder, rt);

        return directory_id(arg_string);
    }
    
    Directory directory(const WebContext&ctx, const RootDir* rt, bool *detected)
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
        
        k   = ctx.find_query("root_dir");
        if(!k.empty())
            rt  = root_dir(k);

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
        return Directory();
    }
    
    Directory directory(const WebContext&ctx, std::string_view arg_name, const RootDir* rt, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected = !arg_string.empty();
        return directory(arg_string, rt);
    }
    
    Directory directory(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, const RootDir* rt, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected = !arg_string.empty();
        return directory(arg_string, rt);
    }
    
    Directory directory_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Directory();
        if(!cdb::exists_directory(*vv))
            return Directory();
        return Directory(*vv);
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
    
    
    Directory directory_key(std::string_view arg_string, const RootDir* rt)
    {
        Folder  folder  = folder_key(arg_string);
        if(!folder)
            return Directory();
        return cdb::first_directory(folder, rt);
    }
    
    Directory directory_key(const WebContext&ctx, std::string_view arg_name, const RootDir* rt, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected = !arg_string.empty();
        return directory_key(arg_string, rt);
    }
    
    Directory directory_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, const RootDir* rt, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected = !arg_string.empty();
        return directory_key(arg_string, rt);
    }
}
