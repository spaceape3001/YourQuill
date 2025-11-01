////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ImagePost.hpp"

#include <yq/mithril/image/ImageArg.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    Image image(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("image");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::image(k);
        }
        return Image();
    }
    
    Image image(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::image(arg_string);
    }
    
    Image image(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::image(arg_string);
    }
}
