////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void page_dev_webvars(WebHtml& h)
    {
        h.title("All WebVariables");
        dev_table(h, web::variable_map());
    }
    
    YQ_INVOKE( reg_webpage<page_dev_webvars>("/dev/web/variables"); )
    
}
