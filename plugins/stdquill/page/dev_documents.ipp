////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    page_dev_documents(WebHtml& h)
    {
        h.title("Listing of Documents");
        dev_table(h, all_documents(Sorted::YES));
    }

    YQ_INVOKE( reg_webpage<page_dev_documents>("/dev/documents"); )
    
}
