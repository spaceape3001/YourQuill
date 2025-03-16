////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ValueArg.hpp"
#include <mithril/web/WebContext.hpp>
#include <mithril/value/ValueCDB.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {
    Value value(std::string_view  arg_string)
    {
        auto vv = to_uint64( arg_string);
        if(!vv)
            return Value();
        if(!cdb::exists_value(*vv))
            return Value();
        return Value(*vv);
    }
    
    Value value(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return value(k);
        k       = ctx.find_query("value");
        if(!k.empty())
            return value(k);
        return Value();
    }
    
    
    Value value(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return value(arg_string);
    }
    
    Value value(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return value(arg_string);
    }
}
