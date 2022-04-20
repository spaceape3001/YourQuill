////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    u_background()
    {
        if(load_background())
            update_css();
    }
    
    YQ_INVOKE( 
        fx_background();
        for(std::string_view f : gBackgroundFiles)
            u_change<u_background>(top_folder(), f);
    )
    
}
