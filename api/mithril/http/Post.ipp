////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Post.hpp"
#include <basic/TextUtils.hpp>
#include <mithril/http/web/WebContext.hpp>

namespace yq {
    namespace post {
        std::string     key(WebContext&ctx)
        {
            ctx.decode_post();
            std::string k  =  ctx.rx_post.first("key");
            if(!is_valid_key(k))
                return std::string();
            return k;
        }
    }
}
