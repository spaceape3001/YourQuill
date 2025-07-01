////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/meta/MetaPropertyArg.hpp>
#include <yq/meta/ArgMeta.hpp>
#include <yq/meta/PropertyMeta.hpp>
#include <yq/meta/Meta.hpp>

#include <mithril/web/WebContext.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {
    const PropertyMeta* property_info(std::string_view  arg_string)
    {
        return property_info_id(arg_string);
    }
    
    const PropertyMeta* property_info(const WebContext&ctx, bool *detected)
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
    
    const PropertyMeta* property_info(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return property_info(arg_string);
    }
    
    const PropertyMeta* property_info(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return property_info(arg_string);
    }

    const PropertyMeta* property_info_id(std::string_view arg_string)
    {
        auto vv = to_uint(arg_string);
        if(!vv)
            return nullptr;
            
        const yq::Meta* m   = yq::Meta::lookup(*vv);
        if(m && m->is_property())
            return static_cast<const PropertyMeta*>(m);

        return nullptr;
    }

    const PropertyMeta* property_info_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return property_info_id(arg_string);
    }
    
    const PropertyMeta* property_info_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return property_info_id(arg_string);
    }
}
