////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    u_page()
    {
        update_page();
    }
    
    YQ_INVOKE( 
        fx_page();
        on_change<u_page>(top_folder(), ".page");
        on_change<u_page>(gSharedPageFile);
    )
    
}
