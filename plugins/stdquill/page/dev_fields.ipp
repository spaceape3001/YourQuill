////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void    page_dev_fields(WebHtml& h)
    {
        h.title() << "All Fields";
        dev_table(h, all_fields(Sorted::YES));
    }
    
    YQ_INVOKE( reg_webpage<page_dev_fields>("/dev/fields"); )
    
}
