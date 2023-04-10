////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SizeDesc.hpp"
#include <mithril/yq/web/WebContext.hpp>

namespace yq {
    namespace mithril {
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

}
