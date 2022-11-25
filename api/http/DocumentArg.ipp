////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentArg.hpp"

#include <basic/TextUtils.hpp>
#include <http/WebContext.hpp>
#include <kernel/DocumentCDB.hpp>

namespace yq {

    namespace arg {

        Document document(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Document{};
                
            Document t   = cdb::document( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_document(i))
                return Document{i};
            return Document{};
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
            return Document{};
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
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_document(i))
                return Document{i};
            return Document{};
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
}
