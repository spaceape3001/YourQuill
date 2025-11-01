////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "CategoryPost.hpp"

#include <yq/mithril/category/CategoryArg.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/category/Category.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    Category category(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("category");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::category_id(k);
        }
        return Category();
    }
    
    Category category(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::category_id(arg_string);
    }
    
    Category category(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::category_id(arg_string);
    }
}
