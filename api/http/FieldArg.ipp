////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldArg.hpp"

#include <basic/TextUtils.hpp>

#include <http/Post.hpp>
#include <http/WebContext.hpp>

#include <kernel/FieldCDB.hpp>

namespace yq {
    namespace arg {
        Field field(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Field{};
                
            Field t   = cdb::field( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_field(i))
                return Field{i};
            return Field{};
        }
        
        Field field(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return field_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return field_key(k);
            
            k       = ctx.find_query("field");
            if(!k.empty())
                return field(k);
            return Field{};
        }
        
        
        Field field(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field(arg_string);
        }
        
        Field field(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field(arg_string);
        }

        Field field_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_field(i))
                return Field{i};
            return Field{};
        }

        Field field_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field_id(arg_string);
        }
        
        Field field_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field_id(arg_string);
        }
        
        Field field_key(std::string_view arg_string)
        {
            return cdb::field(trimmed(arg_string));
        }
        
        Field field_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field_key(arg_string);
        }
        
        Field field_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field_key(arg_string);
        }
    }

    namespace post {
        Field field(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("field");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::field_id(k);
            }
            return Field();
        }
        
        Field field(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::field_id(arg_string);
        }
        
        Field field(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::field_id(arg_string);
        }
    }
}
