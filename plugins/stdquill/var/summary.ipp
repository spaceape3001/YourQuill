////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    var_summary(WebHtml&h)
    {
        Ref<Template>    summary  = gSummary;
        if(summary)
            summary -> execute(h);
    }

    YQ_INVOKE( reg_webvar<var_summary>("summary"); )
}

