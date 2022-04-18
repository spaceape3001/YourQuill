////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    dev_page_users(WebHtml& h)
    {
        h.title() << "All Users";
        dev_table(h, cdb::all_users(Sorted::YES));
    }

    YQ_INVOKE( reg_webpage<dev_page_users>("/dev/users"); )
}
