////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GraphvizArg.hpp"

#include <basic/TextUtils.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/graphviz/GraphvizCDB.hpp>

namespace yq::mithril::arg {
    Graphviz graphviz(std::string_view  arg_string)
    {
        auto vv = to_uint64( arg_string);
        if(!vv)
            return Graphviz();
        if(!cdb::exists_graphviz(*vv))
            return Graphviz();
        return Graphviz(*vv);
    }
    
    Graphviz graphviz(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return graphviz(k);
        k       = ctx.find_query("graphviz");
        if(!k.empty())
            return graphviz(k);
        return Graphviz();
    }
    
    
    Graphviz graphviz(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return graphviz(arg_string);
    }
    
    Graphviz graphviz(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return graphviz(arg_string);
    }
}
