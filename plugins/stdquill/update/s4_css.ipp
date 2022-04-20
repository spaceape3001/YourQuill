////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    fx_css()
    {
        if(gSharedCssFile.empty())
            gSharedCssFile      = wksp::shared(kStdCSS);;
    }

    void    s4_css()
    {
        update_css(DONT_LOCK);
    }

    YQ_INVOKE( 
        fx_css();
        u_stage4<s4_css>(); 
    )
}
