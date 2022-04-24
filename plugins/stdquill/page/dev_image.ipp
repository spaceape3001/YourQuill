////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void page_dev_image(WebHtml& h)
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
    
    YQ_INVOKE( reg_webpage<page_dev_image>("/dev/image"); )
    
}
