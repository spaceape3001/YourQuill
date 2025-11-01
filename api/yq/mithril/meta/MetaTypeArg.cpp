////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/meta/MetaTypeArg.hpp>
#include <yq/meta/ArgMeta.hpp>
#include <yq/meta/TypeMeta.hpp>

#include <yq/mithril/web/WebContext.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {
    const TypeMeta* type_info(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return nullptr;
            
        const TypeMeta* t   = TypeMeta::find(arg_string);
        if(t)
            return t;
            
        return type_info_id(arg_string);
    }
    
    const TypeMeta* type_info(const WebContext&ctx, bool *detected)
    {
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.find_query("id");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return type_info_id(k);
        }
        
        k       = ctx.find_query("key");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return type_info_key(k);
        }
        
        k       = ctx.find_query("type");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return type_info(k);
        }
        return nullptr;
    }
    
    const TypeMeta* type_info(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return type_info(arg_string);
    }
    
    const TypeMeta* type_info(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return type_info(arg_string);
    }

    const TypeMeta* type_info_id(std::string_view arg_string)
    {
        auto vv = to_uint(arg_string);
        if(!vv)
            return nullptr;
        return TypeMeta::find(*vv);
    }

    const TypeMeta* type_info_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return type_info_id(arg_string);
    }
    
    const TypeMeta* type_info_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return type_info_id(arg_string);
    }
    
    const TypeMeta* type_info_key(std::string_view arg_string)
    {
        return TypeMeta::find(trimmed(arg_string));
    }
    
    const TypeMeta* type_info_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return type_info_key(arg_string);
    }
    
    const TypeMeta* type_info_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return type_info_key(arg_string);
    }
}
