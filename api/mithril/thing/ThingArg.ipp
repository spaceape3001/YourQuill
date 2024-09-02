////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ThingArg.hpp"


#include <mithril/web/WebContext.hpp>
#include <mithril/thing/ThingCDB.hpp>

namespace yq::mithril::arg {
    Thing thing(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Thing{};
            
        Thing t   = cdb::thing( arg_string);
        if(t)
            return t;
        return thing_id(arg_string);
    }
    
    Thing thing(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return thing_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return thing_key(k);
        
        k       = ctx.find_query("thing");
        if(!k.empty())
            return thing(k);
        return Thing{};
    }
    
    
    Thing thing(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return thing(arg_string);
    }
    
    Thing thing(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return thing(arg_string);
    }

    Thing thing_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Thing{};
        if(!cdb::exists_thing(*vv))
            return Thing{};
        return Thing{*vv};
    }

    Thing thing_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return thing_id(arg_string);
    }
    
    Thing thing_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return thing_id(arg_string);
    }
    
    Thing thing_key(std::string_view arg_string)
    {
        return cdb::thing(trimmed(arg_string));
    }
    
    Thing thing_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return thing_key(arg_string);
    }
    
    Thing thing_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return thing_key(arg_string);
    }
}
