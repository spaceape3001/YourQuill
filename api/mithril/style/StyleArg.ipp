////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "StyleArg.hpp"


#include <mithril/web/WebContext.hpp>
#include <mithril/style/StyleCDB.hpp>

namespace yq::mithril::arg {
    Style style(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Style();
            
        Style t   = cdb::style( arg_string);
        if(t)
            return t;
            
        return style_id(arg_string);
    }
    
    Style style(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return style_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return style_key(k);
        
        k       = ctx.find_query("style");
        if(!k.empty())
            return style(k);
        return Style();
    }
    
    
    Style style(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return style(arg_string);
    }
    
    Style style(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return style(arg_string);
    }

    Style style_id(std::string_view arg_string)
    {
        auto vv   = to_uint64(arg_string);
        if(!vv)
            return Style();
        if(!cdb::exists_style(*vv))
            return Style();
        return Style(*vv);
    }

    Style style_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return style_id(arg_string);
    }
    
    Style style_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return style_id(arg_string);
    }
    
    Style style_key(std::string_view arg_string)
    {
        return cdb::style(trimmed(arg_string));
    }
    
    Style style_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return style_key(arg_string);
    }
    
    Style style_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return style_key(arg_string);
    }
}
