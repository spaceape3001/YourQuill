////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootArg.hpp"

#include <basic/TextUtils.hpp>
#include <aether/http/web/WebContext.hpp>
#include <aether/kernel/file/RootCDB.hpp>
#include <aether/kernel/wksp/Workspace.hpp>

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
            return root_id(arg_string);
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
            auto vv = to_uint64(arg_string);
            if(!vv)
                return nullptr;
            return wksp::root(*vv);
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
}
