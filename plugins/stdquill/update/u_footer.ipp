////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    u_footer()
    {
        update_footer();
    }
    
    YQ_INVOKE( 
        fx_footer();
        u_change<u_footer>(top_folder(), ".footer");
        u_change<u_footer>(gSharedFooterFile);
    )
    
}
