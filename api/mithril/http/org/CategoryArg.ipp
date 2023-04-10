////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryArg.hpp"

#include <basic/TextUtils.hpp>
#include <mithril/http/web/WebContext.hpp>
#include <mithril/kernel/org/CategoryCDB.hpp>

namespace yq::mithril::arg {
    Category category(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Category{};
            
        Category t   = cdb::category( arg_string);
        if(t)
            return t;
        return category_id(arg_string);
    }
    
    Category category(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return category_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return category_key(k);
        
        k       = ctx.find_query("cat");
        if(!k.empty())
            return category(k);
        return Category{};
    }
    
    
    Category category(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return category(arg_string);
    }
    
    Category category(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return category(arg_string);
    }

    Category category_id(std::string_view arg_string)
    {
        auto vv     = to_uint64(arg_string);
        if(!vv)
            return Category{};
        if(!cdb::exists_category(*vv))
            return Category{};
        return Category{*vv};
    }

    Category category_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return category_id(arg_string);
    }
    
    Category category_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return category_id(arg_string);
    }
    
    Category category_key(std::string_view arg_string)
    {
        return cdb::category(trimmed(arg_string));
    }
    
    Category category_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return category_key(arg_string);
    }
    
    Category category_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return category_key(arg_string);
    }
}
