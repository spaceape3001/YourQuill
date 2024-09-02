////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GroupArg.hpp"


#include <mithril/web/WebContext.hpp>
#include <mithril/group/GroupCDB.hpp>

namespace yq::mithril::arg {
    Group group(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Group{};
            
        Group t   = cdb::group( arg_string);
        if(t)
            return t;
        return group_id(arg_string);
    }
    
    Group group(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return group_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return group_key(k);
        
        k       = ctx.find_query("group");
        if(!k.empty())
            return group(k);
        return Group{};
    }
    
    
    Group group(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return group(arg_string);
    }
    
    Group group(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return group(arg_string);
    }

    Group group_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Group{};
        if(!cdb::exists_group(*vv))
            return Group{};
        return Group{*vv};
    }

    Group group_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return group_id(arg_string);
    }
    
    Group group_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return group_id(arg_string);
    }
    
    Group group_key(std::string_view arg_string)
    {
        return cdb::group(trimmed(arg_string));
    }
    
    Group group_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return group_key(arg_string);
    }
    
    Group group_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return group_key(arg_string);
    }
}
