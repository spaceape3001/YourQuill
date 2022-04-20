////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    fx_background()
    {
        if(!gBackgroundFiles.empty())
            return;
        for(const char* z : Image::kSupportedExtensions)
            gBackgroundFiles.push_back(".background."s + z);
    }

    void    s4_background()
    {
        load_background(DONT_LOCK);
    }
    
    YQ_INVOKE( 
        fx_background();
        u_stage4<s4_background>(); 
    )
}
