////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DrafterArg.hpp"

#include <0/basic/TextUtils.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/drafter/DrafterCDB.hpp>

namespace yq::mithril::arg {
    Drafter drafter(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Drafter();
            
        Drafter t   = cdb::drafter( arg_string);
        if(t)
            return t;
            
        return drafter_id(arg_string);
    }
    
    Drafter drafter(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return drafter_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return drafter_key(k);
        
        k       = ctx.find_query("drafter");
        if(!k.empty())
            return drafter(k);
        return Drafter();
    }
    
    
    Drafter drafter(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return drafter(arg_string);
    }
    
    Drafter drafter(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return drafter(arg_string);
    }

    Drafter drafter_id(std::string_view arg_string)
    {
        auto vv   = to_uint64(arg_string);
        if(!vv)
            return Drafter();
        if(!cdb::exists_drafter(*vv))
            return Drafter();
        return Drafter(*vv);
    }

    Drafter drafter_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return drafter_id(arg_string);
    }
    
    Drafter drafter_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return drafter_id(arg_string);
    }
    
    Drafter drafter_key(std::string_view arg_string)
    {
        return cdb::drafter(trimmed(arg_string));
    }
    
    Drafter drafter_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return drafter_key(arg_string);
    }
    
    Drafter drafter_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return drafter_key(arg_string);
    }
}
