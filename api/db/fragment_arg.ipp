////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "document_arg.hpp"
#include "fragment_arg.hpp"
#include "root_arg.hpp"


namespace yq {
    namespace arg {

        Fragment fragment(std::string_view  arg_string, const Root* rt)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Fragment{};
                
            Document      doc = document_key(arg_string);
            if(doc)
                return cdb::first_fragment(doc, rt);

            return fragment_id(arg_string);
        }
        
        Fragment fragment(const WebContext&ctx, const Root* rt)
        {
            std::string    k;
            
            
            k    = ctx.find_query("id");
            if(!k.empty())
                return fragment_id(k);
            
            k   = ctx.find_query("root");
            if(!k.empty())
                rt  = root(k);

            k       = ctx.find_query("key");
            if(!k.empty())
                return fragment_key(k, rt);
            
            k       = ctx.find_query("fragment");
            if(!k.empty())
                return fragment(k, rt);
            return Fragment{};
        }
        
        Fragment fragment(const WebHtml&h, const Root* rt)
        {
            return fragment(h.context(), rt);
        }
        
        Fragment fragment(const WebContext&ctx, std::string_view arg_name, const Root* rt)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return fragment(arg_string, rt);
        }
        
        Fragment fragment(const WebHtml&h, std::string_view arg_name, const Root* rt)
        {
            return fragment(h.context(), arg_name, rt);
        }
        
        Fragment fragment(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, const Root* rt)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return fragment(arg_string, rt);
        }
        
        Fragment fragment(const WebHtml&h, std::initializer_list<std::string_view> arg_names, const Root* rt)
        {
            return fragment(h.context(), arg_names, rt);
        }

        Fragment fragment_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_fragment(i))
                return Fragment{i};
            return Fragment{};
        }

        Fragment fragment_id(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return fragment_id(arg_string);
        }
        
        Fragment fragment_id(const WebHtml&h, std::string_view arg_name)
        {
            return fragment_id(h.context(), arg_name);
        }
        
        Fragment fragment_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return fragment_id(arg_string);
        }
        
        Fragment fragment_id(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return fragment_id(h.context(), arg_names);
        }
        
        Fragment fragment_key(std::string_view arg_string, const Root* rt)
        {
            Document  doc  = document_key(arg_string);
            if(!doc)
                return Fragment();
            return cdb::first_fragment(doc, rt);
        }
        
        Fragment fragment_key(const WebContext&ctx, std::string_view arg_name, const Root* rt)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return fragment_key(arg_string, rt);
        }
        
        Fragment fragment_key(const WebHtml&h, std::string_view arg_name, const Root* rt)
        {
            return fragment_key(h.context(), arg_name, rt);
        }
        
        Fragment fragment_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, const Root* rt)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return fragment_key(arg_string, rt);
        }
        
        Fragment fragment_key(const WebHtml&h, std::initializer_list<std::string_view> arg_names, const Root* rt)
        {
            return fragment_key(h.context(), arg_names, rt);
        }
    }
}
