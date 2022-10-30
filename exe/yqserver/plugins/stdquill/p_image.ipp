////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    p_image(WebContext& ctx)
    {
        Image   img = image(ctx);
        if(!img)
            throw HttpStatus::BadArgument;

        ByteArray   by   = bytes(img, SizeDesc::Original);
        ctx.tx_content_type = mime_type(img);
        ctx.tx_content      = std::make_shared<ByteArray>(by);
    }

    void    p_thumbnail(WebContext& ctx)
    {
        Image   img = image(ctx);
        if(!img)
            throw HttpStatus::BadArgument;
        
        bool    detected    = false;
        SizeDesc    sz  = size_desc(ctx, "size", &detected);
        if(!detected)
            sz      = ctx.session.icon_size;
        
        ByteArray   by   = bytes(img, sz);
        ctx.tx_content_type = mime_type(img);
        ctx.tx_content      = std::make_shared<ByteArray>(by);
    }


    void    reg_image()
    {
        reg_webpage<p_image>("/image").argument("id", "ID for the image");
        reg_webpage<p_thumbnail>("/thumbnail").argument("id", "ID for the image");

    }
}
