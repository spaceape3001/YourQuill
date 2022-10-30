////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void p_dev_image(WebHtml& h)
    {
        Image   img = image(h);
        if(!img)
            throw HttpStatus::BadArgument;
        
        Fragment    frag    = fragment(img);
        Document    doc     = document(img);
        const Root* rt      = root(frag);
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

    void    reg_dev_image()
    {
        reg_webpage<p_dev_image>("/dev/image");
        reg_webpage<p_dev_images>("/dev/images"); 
    }
}
