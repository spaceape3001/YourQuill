////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/meta/MetaMethodArg.hpp>
#include <yq/meta/ArgMeta.hpp>
#include <yq/meta/MethodMeta.hpp>
#include <yq/meta/Meta.hpp>

#include <mithril/web/WebContext.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {
    const MethodMeta* method_info(std::string_view  arg_string)
    {
        return method_info_id(arg_string);
    }
    
    const MethodMeta* method_info(const WebContext&ctx, bool *detected)
    {
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.find_query("id");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return method_info_id(k);
        }
        
        k       = ctx.find_query("method");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return method_info(k);
        }
        return nullptr;
    }
    
    const MethodMeta* method_info(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return method_info(arg_string);
    }
    
    const MethodMeta* method_info(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return method_info(arg_string);
    }

    const MethodMeta* method_info_id(std::string_view arg_string)
    {
        auto vv = to_uint(arg_string);
        if(!vv)
            return nullptr;
            
        const yq::Meta* m   = yq::Meta::lookup(*vv);
        if(m && m->is_method())
            return static_cast<const MethodMeta*>(m);

        return nullptr;
    }

    const MethodMeta* method_info_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return method_info_id(arg_string);
    }
    
    const MethodMeta* method_info_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return method_info_id(arg_string);
    }
}
