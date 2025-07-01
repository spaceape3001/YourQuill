////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/meta/MetaOperatorArg.hpp>
#include <yq/meta/ArgMeta.hpp>
#include <yq/meta/OperatorMeta.hpp>
#include <yq/meta/Meta.hpp>

#include <mithril/web/WebContext.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {
    const OperatorMeta* operator_info(std::string_view  arg_string)
    {
        return operator_info_id(arg_string);
    }
    
    const OperatorMeta* operator_info(const WebContext&ctx, bool *detected)
    {
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.find_query("id");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return operator_info_id(k);
        }
        
        k       = ctx.find_query("operator");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return operator_info(k);
        }
        return nullptr;
    }
    
    const OperatorMeta* operator_info(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return operator_info(arg_string);
    }
    
    const OperatorMeta* operator_info(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return operator_info(arg_string);
    }

    const OperatorMeta* operator_info_id(std::string_view arg_string)
    {
        auto vv = to_uint(arg_string);
        if(!vv)
            return nullptr;
            
        const yq::Meta* m   = yq::Meta::lookup(*vv);
        if(m && m->is_operator())
            return static_cast<const OperatorMeta*>(m);

        return nullptr;
    }

    const OperatorMeta* operator_info_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return operator_info_id(arg_string);
    }
    
    const OperatorMeta* operator_info_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return operator_info_id(arg_string);
    }
}
