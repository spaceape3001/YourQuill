////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    page_dev_leafs(WebHtml&h)
    {
        h.title("All Leafs");
        dev_table(h, all_leafs(Sorted::YES));
    }
    
    YQ_INVOKE( 
        reg_webpage<page_dev_leafs>("/dev/leafs");
    )
    
}
