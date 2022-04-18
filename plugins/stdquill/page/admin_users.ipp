////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    page_admin_users(WebHtml&h)
    {
        // might have some characteristics?
        h.title() << "Users";
        dev_table(h, cdb::all_users(Sorted::YES));  // STUB
    }

    YQ_INVOKE( reg_webpage<page_admin_users>("/admin/users"); )
}
