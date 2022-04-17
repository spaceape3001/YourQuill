////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "root_arg.hpp"
#include "root.hpp"
#include "workspace.hpp"

namespace yq {
    namespace arg {

        const Root* root(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return nullptr;
                
            const Root* t   = wksp::root( arg_string);
            if(t)
                return t;
            return wksp::root(to_uint64( arg_string).value);
        }
        
        const Root* root(const WebContext&ctx)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return root_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return root_key(k);
            
            k       = ctx.find_query("root");
            if(!k.empty())
                return root(k);
            return nullptr;
        }
        
        const Root* root(const WebHtml&h)
        {
            return root(h.context());
        }
        
        const Root* root(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return root(arg_string);
        }
        
        const Root* root(const WebHtml&h, std::string_view arg_name)
        {
            return root(h.context(), arg_name);
        }
        
        const Root* root(const WebContext& ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return root(arg_string);
        }
        
        const Root* root(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return root(h.context(), arg_names);
        }

        const Root* root_id(std::string_view arg_string)
        {
            return wksp::root(to_uint64(arg_string).value);
        }

        const Root* root_id(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return root_id(arg_string);
        }
        
        const Root* root_id(const WebHtml&h, std::string_view arg_name)
        {
            return root_id(h.context(), arg_name);
        }
        
        const Root* root_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return root_id(arg_string);
        }
        
        const Root* root_id(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return root_id(h.context(), arg_names);
        }
        
        const Root* root_key(std::string_view arg_string)
        {
            return wksp::root(trimmed(arg_string));
        }
        
        const Root* root_key(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return root_key(arg_string);
        }
        
        const Root* root_key(const WebHtml&h, std::string_view arg_name)
        {
            return root_key(h.context(), arg_name);
        }
        
        const Root* root_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return root_key(arg_string);
        }
        
        const Root* root_key(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return root_key(h.context(), arg_names);
        }
    }
}
