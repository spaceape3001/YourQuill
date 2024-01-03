////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/meta/MetaObjectArg.hpp>
#include <0/meta/ObjectInfo.hpp>
#include <0/basic/TextUtils.hpp>
#include <mithril/web/WebContext.hpp>

namespace yq::mithril::arg {
    const ObjectInfo* object_info(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return nullptr;
            
        const ObjectInfo* t   = ObjectInfo::find(arg_string);
        if(t)
            return t;
        
        return object_info_id(arg_string);
    }
    
    const ObjectInfo* object_info(const WebContext&ctx, bool *detected)
    {
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.find_query("id");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return object_info_id(k);
        }
        
        k       = ctx.find_query("key");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return object_info_key(k);
        }
        
        k       = ctx.find_query("object");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return object_info(k);
        }
        return nullptr;
    }
    
    const ObjectInfo* object_info(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return object_info(arg_string);
    }
    
    const ObjectInfo* object_info(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return object_info(arg_string);
    }

    const ObjectInfo* object_info_id(std::string_view arg_string)
    {
        auto vv = to_uint(arg_string);
        if(!vv)
            return nullptr;
        return ObjectInfo::find(*vv);
    }

    const ObjectInfo* object_info_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return object_info_id(arg_string);
    }
    
    const ObjectInfo* object_info_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return object_info_id(arg_string);
    }
    
    const ObjectInfo* object_info_key(std::string_view arg_string)
    {
        return ObjectInfo::find(trimmed(arg_string));
    }
    
    const ObjectInfo* object_info_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return object_info_key(arg_string);
    }
    
    const ObjectInfo* object_info_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return object_info_key(arg_string);
    }
}
