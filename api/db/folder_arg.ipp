////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace arg {

        Folder folder(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Folder{};
                
            Folder t   = cdb::folder( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_folder(i))
                return Folder{i};
            return Folder{};
        }
        
        Folder folder(const WebContext&ctx)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return folder_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return folder_key(k);
            
            k       = ctx.find_query("folder");
            if(!k.empty())
                return folder(k);
            return Folder{};
        }
        
        Folder folder(const WebHtml&h)
        {
            return folder(h.context());
        }
        
        Folder folder(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return folder(arg_string);
        }
        
        Folder folder(const WebHtml&h, std::string_view arg_name)
        {
            return folder(h.context(), arg_name);
        }
        
        Folder folder(const WebContext& ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return folder(arg_string);
        }
        
        Folder folder(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return folder(h.context(), arg_names);
        }

        Folder folder_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_folder(i))
                return Folder{i};
            return Folder{};
        }

        Folder folder_id(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return folder_id(arg_string);
        }
        
        Folder folder_id(const WebHtml&h, std::string_view arg_name)
        {
            return folder_id(h.context(), arg_name);
        }
        
        Folder folder_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return folder_id(arg_string);
        }
        
        Folder folder_id(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return folder_id(h.context(), arg_names);
        }
        
        Folder folder_key(std::string_view arg_string)
        {
            return cdb::folder(trimmed(arg_string));
        }
        
        Folder folder_key(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return folder_key(arg_string);
        }
        
        Folder folder_key(const WebHtml&h, std::string_view arg_name)
        {
            return folder_key(h.context(), arg_name);
        }
        
        Folder folder_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return folder_key(arg_string);
        }
        
        Folder folder_key(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return folder_key(h.context(), arg_names);
        }
    }
}
