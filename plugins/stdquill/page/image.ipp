////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    page_image(WebContext& ctx)
    {
        Image   img = image(ctx);
        if(!img)
            throw HttpStatus::BadArgument;

        ByteArray   by   = bytes(img, SizeDesc::Original);
        ctx.tx_content_type = mime_type(img);
        ctx.tx_content      = std::make_shared<ByteArray>(by);
    }
    
    YQ_INVOKE( reg_webpage<page_image>("/image").argument("id", "ID for the image"); )
}
