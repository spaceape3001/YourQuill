////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FragmentPost.hpp"

#include <yq/mithril/fragment/FragmentArg.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/fragment/Fragment.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    Fragment fragment(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("fragment");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::fragment_id(k);
        }
        return Fragment();
    }
    
    Fragment fragment(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::fragment_id(arg_string);
    }
    
    Fragment fragment(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::fragment_id(arg_string);
    }
}
