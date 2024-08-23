////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/meta/MetaPropertyArg.hpp>
#include <0/meta/ArgInfo.hpp>
#include <0/meta/PropertyInfo.hpp>
#include <0/meta/Meta.hpp>
#include <0/basic/TextUtils.hpp>
#include <mithril/web/WebContext.hpp>

namespace yq::mithril::arg {
    const PropertyInfo* property_info(std::string_view  arg_string)
    {
        return property_info_id(arg_string);
    }
    
    const PropertyInfo* property_info(const WebContext&ctx, bool *detected)
    {
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.find_query("id");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return property_info_id(k);
        }
        
        k       = ctx.find_query("property");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return property_info(k);
        }
        return nullptr;
    }
    
    const PropertyInfo* property_info(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return property_info(arg_string);
    }
    
    const PropertyInfo* property_info(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return property_info(arg_string);
    }

    const PropertyInfo* property_info_id(std::string_view arg_string)
    {
        auto vv = to_uint(arg_string);
        if(!vv)
            return nullptr;
            
        const yq::Meta* m   = yq::Meta::lookup(*vv);
        if(m && m->is_property())
            return static_cast<const PropertyInfo*>(m);

        return nullptr;
    }

    const PropertyInfo* property_info_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return property_info_id(arg_string);
    }
    
    const PropertyInfo* property_info_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return property_info_id(arg_string);
    }
}
