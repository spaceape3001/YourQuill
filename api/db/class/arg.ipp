////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "arg.hpp"
#include "cdb.hpp"

namespace yq {

    namespace arg {
        Class class_(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Class{};
                
            Class t   = cdb::class_( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_class(i))
                return Class{i};
            return Class{};
        }
        
        Class class_(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return class_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return class_key(k);
            
            k       = ctx.find_query("class");
            if(!k.empty())
                return class_(k);
            return Class{};
        }
        
        
        Class class_(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return class_(arg_string);
        }
        
        Class class_(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return class_(arg_string);
        }

        Class class_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_class(i))
                return Class{i};
            return Class{};
        }

        Class class_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return class_id(arg_string);
        }
        
        Class class_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return class_id(arg_string);
        }
        
        Class class_key(std::string_view arg_string)
        {
            return cdb::class_(trimmed(arg_string));
        }
        
        Class class_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return class_key(arg_string);
        }
        
        Class class_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return class_key(arg_string);
        }
    }
}