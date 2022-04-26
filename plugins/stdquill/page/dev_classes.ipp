////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    page_dev_classes(WebHtml&h)
    {
        h.title() << "All Classes";
        dev_table(h, all_classes(Sorted::YES));
    }
    
    YQ_INVOKE( reg_webpage<page_dev_classes>("/dev/classes"); )
    
}
