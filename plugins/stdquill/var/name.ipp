////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    var_name(WebHtml& h)
    {
        html_escape_write(h, wksp::name());
    }

    YQ_INVOKE( reg_webvar<var_name>("name"); )
}
