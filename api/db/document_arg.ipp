////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "document_arg.hpp"

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
        
        Document document(const WebContext&ctx)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return document_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return document_key(k);
            
            k       = ctx.find_query("document");
            if(!k.empty())
                return document(k);
            return Document{};
        }
        
        Document document(const WebHtml&h)
        {
            return document(h.context());
        }
        
        Document document(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return document(arg_string);
        }
        
        Document document(const WebHtml&h, std::string_view arg_name)
        {
            return document(h.context(), arg_name);
        }
        
        Document document(const WebContext& ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return document(arg_string);
        }
        
        Document document(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return document(h.context(), arg_names);
        }

        Document document_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_document(i))
                return Document{i};
            return Document{};
        }

        Document document_id(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return document_id(arg_string);
        }
        
        Document document_id(const WebHtml&h, std::string_view arg_name)
        {
            return document_id(h.context(), arg_name);
        }
        
        Document document_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return document_id(arg_string);
        }
        
        Document document_id(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return document_id(h.context(), arg_names);
        }
        
        Document document_key(std::string_view arg_string)
        {
            return cdb::document(trimmed(arg_string));
        }
        
        Document document_key(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return document_key(arg_string);
        }
        
        Document document_key(const WebHtml&h, std::string_view arg_name)
        {
            return document_key(h.context(), arg_name);
        }
        
        Document document_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return document_key(arg_string);
        }
        
        Document document_key(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return document_key(h.context(), arg_names);
        }

    }
}
