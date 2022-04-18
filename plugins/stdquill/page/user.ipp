////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    page_user(WebHtml& h)
    {
        User    u   = arg::user(h);
        if(!u)
            throw HttpStatus::BadArgument;
        
        h.title() << "User: " << cdb::name(u);
        h.p() << "Fill in the blank... TODO!";
    }

    YQ_INVOKE( reg_webpage<page_user>("/user"); )
}
