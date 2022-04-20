////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    dev_fragment_image(WebContext& ctx)
    {
        Fragment x  = fragment(ctx, nullptr);
        if(!x)  
            throw HttpStatus::BadArgument;
    
        ctx.tx_content_type = mimeTypeForExt( suffix(x));
        ctx.tx_content      = std::make_shared<ByteArray>(frag_bytes(x));
    }
    
    YQ_INVOKE( reg_webpage<dev_fragment_image>("/dev/fragment/image").argument("id", "ID for fragment"); )
}
