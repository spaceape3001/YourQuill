////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    page_dev_fragments(WebHtml& h)
    {
        h.title("Listing of Fragments");
        dev_table(h, all_fragments(Sorted::YES));
    }

    YQ_INVOKE( reg_webpage<page_dev_fragments>("/dev/fragments"); )
    
}
