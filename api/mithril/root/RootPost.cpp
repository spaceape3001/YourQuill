////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "RootPost.hpp"

#include <mithril/root/RootArg.hpp>
#include <mithril/web/WebContext.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    const RootDir* root_dir(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("root_dir");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::root_id(k);
        }
        return nullptr;
    }
    
    const RootDir* root_dir(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::root_id(arg_string);
    }
    
    const RootDir* root_dir(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::root_id(arg_string);
    }
}
