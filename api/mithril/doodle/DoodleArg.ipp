////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DoodleArg.hpp"

#include <0/basic/TextUtils.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/doodle/DoodleCDB.hpp>

namespace yq::mithril::arg {
    Doodle doodle(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Doodle();
            
        Doodle t   = cdb::doodle( arg_string);
        if(t)
            return t;
            
        return doodle_id(arg_string);
    }
    
    Doodle doodle(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return doodle_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return doodle_key(k);
        
        k       = ctx.find_query("doodle");
        if(!k.empty())
            return doodle(k);
        return Doodle();
    }
    
    
    Doodle doodle(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return doodle(arg_string);
    }
    
    Doodle doodle(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return doodle(arg_string);
    }

    Doodle doodle_id(std::string_view arg_string)
    {
        auto vv   = to_uint64(arg_string);
        if(!vv)
            return Doodle();
        if(!cdb::exists_doodle(*vv))
            return Doodle();
        return Doodle(*vv);
    }

    Doodle doodle_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return doodle_id(arg_string);
    }
    
    Doodle doodle_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return doodle_id(arg_string);
    }
    
    Doodle doodle_key(std::string_view arg_string)
    {
        return cdb::doodle(trimmed(arg_string));
    }
    
    Doodle doodle_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return doodle_key(arg_string);
    }
    
    Doodle doodle_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return doodle_key(arg_string);
    }
}
