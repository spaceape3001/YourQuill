////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FragmentArg.hpp"


#include <mithril/document/DocumentArg.hpp>
#include <mithril/root/RootArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>

namespace yq::mithril::arg {

    Fragment fragment(std::string_view  arg_string, const RootDir* rt)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Fragment{};
            
        Document      doc = document_key(arg_string);
        if(doc)
            return cdb::first_fragment(doc, rt);

        return fragment_id(arg_string);
    }
    
    Fragment fragment(const WebContext&ctx, const RootDir* rt, bool *detected)
    {
        if(detected)
            *detected   = false;
        
        std::string k    = ctx.find_query("id");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return fragment_id(k);
        }
        
        k   = ctx.find_query("root");
        if(!k.empty())
            rt  = root_dir(k);

        k       = ctx.find_query("key");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return fragment_key(k, rt);
        }
        
        k       = ctx.find_query("fragment");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return fragment(k, rt);
        }
        return Fragment{};
    }
    
    Fragment fragment(const WebContext&ctx, std::string_view arg_name, const RootDir* rt, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return fragment(arg_string, rt);
    }
    
    Fragment fragment(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, const RootDir* rt, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return fragment(arg_string, rt);
    }
    
    Fragment fragment_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Fragment();
        if(!cdb::exists_fragment(*vv))
            return Fragment();
        return Fragment(*vv);
    }

    Fragment fragment_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return fragment_id(arg_string);
    }
    
    Fragment fragment_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return fragment_id(arg_string);
    }
    
    Fragment fragment_key(std::string_view arg_string, const RootDir* rt)
    {
        Document  doc  = document_key(arg_string);
        if(!doc)
            return Fragment();
        return cdb::first_fragment(doc, rt);
    }
    
    Fragment fragment_key(const WebContext&ctx, std::string_view arg_name, const RootDir* rt, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return fragment_key(arg_string, rt);
    }
    
    Fragment fragment_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, const RootDir* rt, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return fragment_key(arg_string, rt);
    }
}
