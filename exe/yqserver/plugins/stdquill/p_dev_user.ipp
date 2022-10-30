////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    p_dev_users(WebHtml& h)
    {
        h.title() << "All Users";
        dev_table(h, cdb::all_users(Sorted::YES));
    }

    void    reg_dev_user()
    {
        reg_webpage<p_dev_users>("/dev/users");
    }
}
