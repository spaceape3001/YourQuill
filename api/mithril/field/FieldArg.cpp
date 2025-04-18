////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FieldArg.hpp"

#include <mithril/web/WebContext.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {
    Field field(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Field();
            
        Field t   = cdb::field( arg_string);
        if(t)
            return t;
        return field_id(arg_string);
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
        return Field();
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
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Field();
        if(!cdb::exists_field(*vv))
            return Field();
        return Field(*vv);
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
