////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    fx_page()
    {
        if(gSharedPageFile.empty())
            gSharedPageFile     = wksp::shared(kStdPage);
    }
    
    void    s4_page()
    {
        update_page(DONT_LOCK);
    }
    
    YQ_INVOKE( 
        fx_page();
        u_stage4<s4_page>();
    )
}
