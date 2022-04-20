////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    u_index()
    {
        update_index();
    }
    
    YQ_INVOKE( 
        fx_index();
        for(std::string_view k : kIndexFiles)
            u_change<u_index>(top_folder(), k);
        u_change<u_index>(gSharedIndexFile);
    )
}
