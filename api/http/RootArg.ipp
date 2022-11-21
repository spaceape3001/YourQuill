////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootArg.hpp"

#include <basic/TextUtils.hpp>
#include <http/Post.hpp>
#include <http/WebContext.hpp>
#include <kernel/RootCDB.hpp>
#include <kernel/Workspace.hpp>

namespace yq {

    namespace arg {

        const Root* root(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return nullptr;
                
            const Root* t   = wksp::root( arg_string);
            if(t)
                return t;
            return wksp::root(to_uint64( arg_string).value);
        }
        
        const Root* root(const WebContext&ctx, bool *detected)
        {
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.find_query("id");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return root_id(k);
            }
            
            k       = ctx.find_query("key");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return root_key(k);
            }
            
            k       = ctx.find_query("root");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return root(k);
            }
            return nullptr;
        }
        
        const Root* root(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return root(arg_string);
        }
        
        const Root* root(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return root(arg_string);
        }

        const Root* root_id(std::string_view arg_string)
        {
            return wksp::root(to_uint64(arg_string).value);
        }

        const Root* root_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return root_id(arg_string);
        }
        
        const Root* root_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return root_id(arg_string);
        }
        
        const Root* root_key(std::string_view arg_string)
        {
            return wksp::root(trimmed(arg_string));
        }
        
        const Root* root_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return root_key(arg_string);
        }
        
        const Root* root_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return root_key(arg_string);
        }
    }

    namespace post {
        const Root* root(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("root");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::root_id(k);
            }
            return nullptr;
        }
        
        const Root* root(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::root_id(arg_string);
        }
        
        const Root* root(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::root_id(arg_string);
        }
    }
}
