////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "tag_arg.hpp"
#include "tag_cdb.hpp"

namespace yq {

    namespace arg {
        Tag tag(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Tag{};
                
            Tag t   = cdb::tag( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_tag(i))
                return Tag{i};
            return Tag{};
        }
        
        Tag tag(const WebContext&ctx)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return tag_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return tag_key(k);
            
            k       = ctx.find_query("tag");
            if(!k.empty())
                return tag(k);
            return Tag{};
        }
        
        Tag tag(const WebHtml&h)
        {
            return tag(h.context());
        }
        
        Tag tag(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return tag(arg_string);
        }
        
        Tag tag(const WebHtml&h, std::string_view arg_name)
        {
            return tag(h.context(), arg_name);
        }
        
        Tag tag(const WebContext& ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return tag(arg_string);
        }
        
        Tag tag(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return tag(h.context(), arg_names);
        }

        Tag tag_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_tag(i))
                return Tag{i};
            return Tag{};
        }

        Tag tag_id(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return tag_id(arg_string);
        }
        
        Tag tag_id(const WebHtml&h, std::string_view arg_name)
        {
            return tag_id(h.context(), arg_name);
        }
        
        Tag tag_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return tag_id(arg_string);
        }
        
        Tag tag_id(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return tag_id(h.context(), arg_names);
        }
        
        Tag tag_key(std::string_view arg_string)
        {
            return cdb::tag(trimmed(arg_string));
        }
        
        Tag tag_key(const WebContext&ctx, std::string_view arg_name)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            return tag_key(arg_string);
        }
        
        Tag tag_key(const WebHtml&h, std::string_view arg_name)
        {
            return tag_key(h.context(), arg_name);
        }
        
        Tag tag_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            return tag_key(arg_string);
        }
        
        Tag tag_key(const WebHtml&h, std::initializer_list<std::string_view> arg_names)
        {
            return tag_key(h.context(), arg_names);
        }
    }
}
