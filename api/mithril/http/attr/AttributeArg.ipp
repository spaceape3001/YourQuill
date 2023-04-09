////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AttributeArg.hpp"

#include <basic/TextUtils.hpp>
#include <mithril/http/web/WebContext.hpp>
#include <mithril/kernel/attr/AttributeCDB.hpp>

namespace yq {
    namespace arg {
        Attribute attribute(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Attribute{};
            uint64_t    i = to_uint64( arg_string).value_or(0);
            if(cdb::exists_attribute(i))
                return Attribute{i};
            return Attribute{};
        }
        
        Attribute attribute(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return attribute(k);
            k       = ctx.find_query("attr");
            if(!k.empty())
                return attribute(k);
            return Attribute{};
        }
        
        
        Attribute attribute(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return attribute(arg_string);
        }
        
        Attribute attribute(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return attribute(arg_string);
        }
    }
}
