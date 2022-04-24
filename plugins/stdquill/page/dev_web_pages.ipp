////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void page_dev_webpages(WebHtml& h)
    {
        h.title("All WebPages");
        dev_table(h, web::page_map(), true);
    }
    
    YQ_INVOKE( reg_webpage<page_dev_webpages>("/dev/web/pages"); )
    
}
