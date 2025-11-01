////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Post.hpp"

#include <yq/mithril/web/WebContext.hpp>
#include <yq/text/match.hpp>

namespace yq::mithril::post {
    std::string     key(WebContext&ctx)
    {
        ctx.decode_post();
        std::string k  =  ctx.rx_post.first("key");
        if(!is_valid_key(k))
            return std::string();
        return k;
    }
}
