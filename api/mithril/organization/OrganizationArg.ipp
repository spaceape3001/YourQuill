////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "OrganizationArg.hpp"

#include <0/basic/TextUtils.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/organization/OrganizationCDB.hpp>

namespace yq::mithril::arg {
    Organization organization(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Organization{};
            
        Organization t   = cdb::organization( arg_string);
        if(t)
            return t;
        return organization_id(arg_string);
    }
    
    Organization organization(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return organization_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return organization_key(k);
        
        k       = ctx.find_query("organization");
        if(!k.empty())
            return organization(k);
        return Organization{};
    }
    
    
    Organization organization(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return organization(arg_string);
    }
    
    Organization organization(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return organization(arg_string);
    }

    Organization organization_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Organization{};
        if(!cdb::exists_organization(*vv))
            return Organization{};
        return Organization{*vv};
    }

    Organization organization_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return organization_id(arg_string);
    }
    
    Organization organization_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return organization_id(arg_string);
    }
    
    Organization organization_key(std::string_view arg_string)
    {
        return cdb::organization(trimmed(arg_string));
    }
    
    Organization organization_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return organization_key(arg_string);
    }
    
    Organization organization_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return organization_key(arg_string);
    }
}
