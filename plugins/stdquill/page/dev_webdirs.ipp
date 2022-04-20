////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    page_dev_webdirs(WebHtml&h)
    {
        h.title("All WebDirs");
        dev_table(h, web::directory_map());
    }
    
    YQ_INVOKE( reg_webpage<page_dev_webdirs>("/dev/webdirs"); )
    
}
