////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UserArg.hpp"

#include <basic/TextUtils.hpp>
#include <http/Post.hpp>
#include <http/WebContext.hpp>
#include <kernel/UserCDB.hpp>

namespace yq {

    namespace arg {
        User user(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return User{};
                
            User t   = cdb::user( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_user(i))
                return User{i};
            return User{};
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
            return User{};
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
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_user(i))
                return User{i};
            return User{};
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

    namespace post {
        User user(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("user");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::user_id(k);
            }
            return User();
        }
        
        User user(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::user_id(arg_string);
        }
        
        User user(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::user_id(arg_string);
        }
    }
}
