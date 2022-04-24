////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    
    void page_dev_webglobs(WebHtml& h)
    {
        h.title("All WebGlobs");
        dev_table(h, web::glob_map());
    }
    
    YQ_INVOKE( reg_webpage<page_dev_webglobs>("/dev/web/globs"); )
    
}
