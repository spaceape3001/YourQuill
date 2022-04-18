////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    page_dev_folders(WebHtml& h)
    {
        h.title("Listing of Folders");
        dev_table(h, all_folders(Sorted::YES));
    }

    YQ_INVOKE( reg_webpage<page_dev_folders>("/dev/folders"); )
    
}
