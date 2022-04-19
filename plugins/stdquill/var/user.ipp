////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    var_user(WebHtml& h)
    {
        auto& ctx = h.context().session;
        if(!ctx.logged_in)
            h << "(anonymous)";
        else
            h << ctx.username;
    }

    YQ_INVOKE( reg_webvar<var_user>("user"); )
}

