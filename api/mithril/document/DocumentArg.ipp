////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentArg.hpp"

#include <basic/TextUtils.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/document/DocumentCDB.hpp>

namespace yq::mithril::arg {

    Document document(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Document();
            
        Document t   = cdb::document( arg_string);
        if(t)
            return t;
        return document_id(arg_string);
    }
    
    Document document(const WebContext&ctx, bool *detected)
    {
        if(detected)
            *detected   = false;
    
        std::string    k    = ctx.find_query("id");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return document_id(k);
        }
        
        k       = ctx.find_query("key");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return document_key(k);
        }
        
        k       = ctx.find_query("document");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return document(k);
        }
        return Document();
    }
    
    Document document(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return document(arg_string);
    }
    
    Document document(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return document(arg_string);
    }
    
    Document document_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Document();
        if(!cdb::exists_document(*vv))
            return Document();
        return Document(*vv);
    }

    Document document_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return document_id(arg_string);
    }
    
    Document document_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return document_id(arg_string);
    }
    
    Document document_key(std::string_view arg_string)
    {
        return cdb::document(trimmed(arg_string));
    }
    
    Document document_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return document_key(arg_string);
    }
    
    Document document_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return document_key(arg_string);
    }
}
