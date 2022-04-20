////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    fx_footer()
    {
        if(gSharedFooterFile.empty())
            gSharedFooterFile   = wksp::shared(kStdFooter);
    }
    
    void    s4_footer()
    {
        update_footer(DONT_LOCK);
    }
    
    YQ_INVOKE( 
        fx_footer();
        u_stage4<s4_footer>();
    )
    
}
