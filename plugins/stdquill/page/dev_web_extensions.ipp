////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void page_dev_webexts(WebHtml& h)
    {
        h.title("All WebExts");
        dev_table(h, web::extension_map());
    }
    
    YQ_INVOKE( reg_webpage<page_dev_webexts>("/dev/web/extensions"); )
    
}
