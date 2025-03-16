////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TagArg.hpp"
#include <mithril/web/WebContext.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {
    Tag tag(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Tag{};
            
        Tag t   = cdb::find_tag( arg_string);
        if(t)
            return t;
        return tag_id(arg_string);
    }
    
    Tag tag(const WebContext&ctx, bool *detected)
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
    
    
    Tag tag(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return tag(arg_string);
    }
    
    Tag tag(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return tag(arg_string);
    }

    Tag tag_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Tag{};
        if(!cdb::exists_tag(*vv))
            return Tag{};
        return Tag{*vv};
    }

    Tag tag_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return tag_id(arg_string);
    }
    
    Tag tag_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return tag_id(arg_string);
    }
    
    Tag tag_key(std::string_view arg_string)
    {
        return cdb::find_tag(trimmed(arg_string));
    }
    
    Tag tag_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return tag_key(arg_string);
    }
    
    Tag tag_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return tag_key(arg_string);
    }
}
