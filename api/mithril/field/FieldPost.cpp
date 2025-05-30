////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FieldPost.hpp"

#include <mithril/field/FieldArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/field/Field.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    Field field(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("field");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::field_id(k);
        }
        return Field();
    }
    
    Field field(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::field_id(arg_string);
    }
    
    Field field(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::field_id(arg_string);
    }
}
