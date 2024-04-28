////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
        
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


    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------


        void p_dev_image(WebHtml& h)
        {
            Image   img = image(h);
            if(!img)
                throw HttpStatus::BadArgument;
            
            Fragment    frag    = fragment(img);
            Document    doc     = document(img);
            const RootDir* rt      = root_dir(frag);
            if(!rt)
                throw HttpStatus::BadArgument;
            
            h.title() << "Image [" << rt->key << "](" << label(doc) << ")";
            h << img;
        }

        void p_dev_images(WebHtml& h)
        {
            h.title("All Images");
            dev_table(h, all_images());
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------
    
        void reg_image_pages()
        {
            reg_webpage<p_image>("/image").argument("id", "ID for the image");
            reg_webpage<p_thumbnail>("/thumbnail").argument("id", "ID for the image");

            reg_webpage<p_dev_image>("/dev/image");
            reg_webpage<p_dev_images>("/dev/images"); 
        }
}
