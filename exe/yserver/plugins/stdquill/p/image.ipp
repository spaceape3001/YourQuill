////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/image/ImageJson.hpp>
#include <mithril/image/ImageSearch.hpp>


namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_image(WebContext& ctx)
        {
            Image    v   = arg::image(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_image_key(WebContext& ctx)
        {
            Image    v   = arg::image(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_images(WebContext& ctx)
        {
            ImageVector  ret = search(ctx, IMAGE);
            return json{
                { "images", json_(ret) }
            };
        }
        
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
            reg_webpage<p_api_image>("/api/image").argument("ID", "Image ID");
            reg_webpage<p_api_image_key>("/api/image/key").argument("ID", "Image ID");
            reg_webpage<p_api_images>("/api/images");

            reg_webpage<p_image>("/image").argument("id", "ID for the image");
            reg_webpage<p_thumbnail>("/thumbnail").argument("id", "ID for the image");

            reg_webpage<p_dev_image>("/dev/image");
            reg_webpage<p_dev_images>("/dev/images"); 
        }
}
