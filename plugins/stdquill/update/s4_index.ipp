////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    fx_index()
    {
        if(gSharedIndexFile.empty())
            gSharedIndexFile    = wksp::shared(kStdIndex);
    }

    void    s4_index()
    {
        update_index(DONT_LOCK);
    }
    
    YQ_INVOKE( 
        fx_index();
        u_stage4<s4_index>(); 
    )
    
}
