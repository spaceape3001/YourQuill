////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "RootArg.hpp"
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/root/RootCDB.hpp>
#include <yq/mithril/wksp/Workspace.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {

    const RootDir* root_dir(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return nullptr;
        const RootDir* t   = wksp::root_dir( arg_string);
        if(t)
            return t;
        return root_id(arg_string);
    }
    
    const RootDir* root_dir(const WebContext&ctx, bool *detected)
    {
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.find_query("id");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return root_id(k);
        }
        
        k       = ctx.find_query("key");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return root_key(k);
        }
        
        k       = ctx.find_query("root_dir");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return root_dir(k);
        }
        return nullptr;
    }
    
    const RootDir* root_dir(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return root_dir(arg_string);
    }
    
    const RootDir* root_dir(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return root_dir(arg_string);
    }

    const RootDir* root_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return nullptr;
        return wksp::root_dir(*vv);
    }

    const RootDir* root_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return root_id(arg_string);
    }
    
    const RootDir* root_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return root_id(arg_string);
    }
    
    const RootDir* root_key(std::string_view arg_string)
    {
        return wksp::root_dir(trimmed(arg_string));
    }
    
    const RootDir* root_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return root_key(arg_string);
    }
    
    const RootDir* root_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return root_key(arg_string);
    }
}
