////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


namespace yq {
    namespace arg {

        Directory directory(std::string_view  arg_string, const Root* rt)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Directory{};
                
            Folder      folder = folder_key(arg_string);
            if(folder)
                return cdb::first_directory(folder, rt);

            return directory_id(arg_string);
        }
        
        Directory directory(const WebContext&ctx, const Root* rt)
        {
            std::string    k;
            
            
            k    = ctx.find_query("id");
            if(!k.empty())
                return directory_id(k);
            
            k   = ctx.find_query("root");
            if(!k.empty())
                rt  = root(k);

            k       = ctx.find_query("key");
            if(!k.empty())
                return directory_key(k, rt);
            
            k       = ctx.find_query("directory");
            if(!k.empty())
                return directory(k, rt);
            return Directory{};
        }
        
        Directory directory(const WebHtml&h, const Root* rt)
        {
            return directory(h.context(), rt);
        }
        
        Directory directory(const WebContext&ctx, std::string_view arg_name, const Root* rt)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return directory(arg_string, rt);
        }
        
        Directory directory(const WebHtml&h, std::string_view arg_name, const Root* rt)
        {
            return directory(h.context(), arg_name, rt);
        }
        
        Directory directory(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, const Root* rt)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return directory(arg_string, rt);
        }
        
        Directory directory(const WebHtml&h, std::initializer_list<std::string_view> arg_names, const Root* rt)
        {
            return directory(h.context(), arg_names, rt);
        }

        Directory directory_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_directory(i))
                return Directory{i};
            return Directory{};
        }

        Directory directory_id(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return directory_id(arg_string);
        }
        
        Directory directory_id(const WebHtml&h, std::string_view arg_name)
        {
            return directory_id(h.context(), arg_name);
        }
        
        Directory directory_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return directory_id(arg_string);
        }
        
        Directory directory_id(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return directory_id(h.context(), arg_names);
        }
        
        Directory directory_key(std::string_view arg_string, const Root* rt)
        {
            Folder  folder  = folder_key(arg_string);
            if(!folder)
                return Directory();
            return cdb::first_directory(folder, rt);
        }
        
        Directory directory_key(const WebContext&ctx, std::string_view arg_name, const Root* rt)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return directory_key(arg_string, rt);
        }
        
        Directory directory_key(const WebHtml&h, std::string_view arg_name, const Root* rt)
        {
            return directory_key(h.context(), arg_name, rt);
        }
        
        Directory directory_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, const Root* rt)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return directory_key(arg_string, rt);
        }
        
        Directory directory_key(const WebHtml&h, std::initializer_list<std::string_view> arg_names, const Root* rt)
        {
            return directory_key(h.context(), arg_names, rt);
        }
    }
}
