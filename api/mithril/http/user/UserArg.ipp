////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UserArg.hpp"

#include <basic/TextUtils.hpp>
#include <mithril/http/web/WebContext.hpp>
#include <mithril/kernel/user/UserCDB.hpp>

namespace yq::mithril::arg {
    User user(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return User();
            
        User t   = cdb::user( arg_string);
        if(t)
            return t;
        return user_id(arg_string);
    }
    
    User user(const WebContext&ctx, bool *detected)
    {
        if(detected)
            *detected = false;
            
        std::string    k    = ctx.find_query("id");
        if(!k.empty()){
            if(detected)
                *detected = true;
            return user_id(k);
        }
        
        k       = ctx.find_query("key");
        if(!k.empty()){
            if(detected)
                *detected = true;
            return user_key(k);
        }
        
        k       = ctx.find_query("user");
        if(!k.empty()){
            if(detected)
                *detected = true;
            return user(k);
        }
        return User();
    }
    
    User user(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return user(arg_string);
    }
    
    User user(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return user(arg_string);
    }
    

    User user_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return User();
        if(!cdb::exists_user(*vv))
            return User();
        return User(*vv);
    }

    User user_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return user_id(arg_string);
    }
    
    User user_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return user_id(arg_string);
    }
    
    User user_key(std::string_view arg_string)
    {
        return cdb::user(trimmed(arg_string));
    }
    
    User user_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return user_key(arg_string);
    }
    
    User user_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return user_key(arg_string);
    }
}
