////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "user_arg.hpp"
#include "user_cdb.hpp"


namespace yq {
    namespace arg {
        User user(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return User{};
                
            User t   = cdb::user( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_user(i))
                return User{i};
            return User{};
        }
        
        User user(const WebContext&ctx)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return user_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return user_key(k);
            
            k       = ctx.find_query("user");
            if(!k.empty())
                return user(k);
            return User{};
        }
        
        User user(const WebHtml&h)
        {
            return user(h.context());
        }
        
        User user(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return user(arg_string);
        }
        
        User user(const WebHtml&h, std::string_view arg_name)
        {
            return user(h.context(), arg_name);
        }
        
        User user(const WebContext& ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return user(arg_string);
        }
        
        User user(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return user(h.context(), arg_names);
        }

        User user_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_user(i))
                return User{i};
            return User{};
        }

        User user_id(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return user_id(arg_string);
        }
        
        User user_id(const WebHtml&h, std::string_view arg_name)
        {
            return user_id(h.context(), arg_name);
        }
        
        User user_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return user_id(arg_string);
        }
        
        User user_id(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return user_id(h.context(), arg_names);
        }
        
        User user_key(std::string_view arg_string)
        {
            return cdb::user(trimmed(arg_string));
        }
        
        User user_key(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return user_key(arg_string);
        }
        
        User user_key(const WebHtml&h, std::string_view arg_name)
        {
            return user_key(h.context(), arg_name);
        }
        
        User user_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return user_key(arg_string);
        }
        
        User user_key(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return user_key(h.context(), arg_names);
        }
    }
}
