////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    p_leaf(WebHtml& h)
    {
        //  BIG FAT TODO!
    }

    void    reg_leaf()
    {
        reg_webpage<p_leaf>("/leaf").argument("id", "Leaf ID");
    }
}