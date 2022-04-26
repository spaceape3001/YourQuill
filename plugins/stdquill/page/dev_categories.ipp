////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_dev_categories(WebHtml&h)
    {
        h.title() << "All Categories!";
        dev_table(h, all_categories(Sorted::YES));
    }
    
    YQ_INVOKE( reg_webpage<page_dev_categories>("/dev/categories"); )
}
