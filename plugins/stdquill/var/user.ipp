////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    var_user(Stream&str, WebContext& ctx)
    {
        if(!ctx.session.logged_in)
            str << "(anonymous)";
        else
            str << ctx.session.username;
    }

    YQ_INVOKE( reg_webvar<var_user>("user"); )
}

