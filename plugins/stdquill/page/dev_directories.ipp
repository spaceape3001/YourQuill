////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_dev_directories(WebHtml& h)
    {
        h.title("All Directories");
        dev_table(h, all_directories(Sorted::YES));
    }
    
    YQ_INVOKE( reg_webpage<page_dev_directories>("/dev/directories");)
    
}
