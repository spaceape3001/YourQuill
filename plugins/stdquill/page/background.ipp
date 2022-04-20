////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_background(WebContext& ctx)
    {
        Ref<TypedBytes>     data = gBackground;
        if(!data){
            ctx.status  = HttpStatus::ServiceUnavailable;
            return;
        }
        
        ctx.tx_content_type = data -> type;
        ctx.tx_content      = data -> data;
        ctx.tx_header("Date", data->modified);
        ctx.status          = HttpStatus::Success;
    }
    
    
    YQ_INVOKE( reg_webpage<page_background>("/background"); )
    
}
