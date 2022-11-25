////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldArg.hpp"

#include <basic/TextUtils.hpp>
#include <http/web/WebContext.hpp>
#include <kernel/agw/FieldCDB.hpp>

namespace yq {
    namespace arg {
        agw::Field field(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return agw::Field{};
                
            agw::Field t   = cdb::field( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_field(i))
                return agw::Field{i};
            return agw::Field{};
        }
        
        agw::Field field(const WebContext&ctx, bool *detected)
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
            return agw::Field{};
        }
        
        
        agw::Field field(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field(arg_string);
        }
        
        agw::Field field(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field(arg_string);
        }

        agw::Field field_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_field(i))
                return agw::Field{i};
            return agw::Field{};
        }

        agw::Field field_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field_id(arg_string);
        }
        
        agw::Field field_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field_id(arg_string);
        }
        
        agw::Field field_key(std::string_view arg_string)
        {
            return cdb::field(trimmed(arg_string));
        }
        
        agw::Field field_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field_key(arg_string);
        }
        
        agw::Field field_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field_key(arg_string);
        }
    }
}
