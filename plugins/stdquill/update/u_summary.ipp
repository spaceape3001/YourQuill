////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    u_summary()
    {
        update_summary();
    }
    
    YQ_INVOKE( 
        fx_summary();
        u_change<u_summary>(top_folder(), ".summary");
        u_change<u_summary>(gSharedSummaryFile);
    )
    
}
