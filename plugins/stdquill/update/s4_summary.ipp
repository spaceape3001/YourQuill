////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    fx_summary()
    {
        if(gSharedSummaryFile.empty())
            gSharedSummaryFile  = wksp::shared(kStdSummary);;
    }
    
    void    s4_summary()
    {
        update_summary(DONT_LOCK);
    }
    
    YQ_INVOKE( 
        fx_summary();
        u_stage4<s4_summary>();
    )
    
}
