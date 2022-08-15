////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagArg.hpp"

#include <basic/TextUtils.hpp>
#include <http/web/WebContext.hpp>
#include <kernel/org/TagCDB.hpp>

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
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_tag(i))
                return Tag{i};
            return Tag{};
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
            return cdb::tag(trimmed(arg_string));
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
}
