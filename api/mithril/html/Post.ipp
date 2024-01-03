////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Post.hpp"
#include <0/basic/TextUtils.hpp>
#include <mithril/web/WebContext.hpp>

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
