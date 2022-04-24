////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "size_desc_arg.hpp"

namespace yq {
    namespace arg {
        SizeDesc    size_desc(std::string_view k)
        {
            return SizeDesc(k);
        }
        
        SizeDesc    size_desc(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return size_desc(arg_string);
        }
        
        SizeDesc    size_desc(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return size_desc(arg_string);
        }
    }
}

