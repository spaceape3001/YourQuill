////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    p_user(WebHtml& h)
    {
        User    u   = arg::user(h);
        if(!u)
            throw HttpStatus::BadArgument;
        
        h.title() << "User: " << cdb::name(u);
        h.p() << "Fill in the blank... TODO!";
    }

    void    reg_user()
    {
        reg_webpage<p_user>("/user");
    }
}
