////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_dev_hello(WebHtml& out)
    {
        out.title("HELLO WORLD!");
        
        auto _t     = out.table();
        
        out.kvrow("Method") << out.context().method.key() << "\n";
        out.kvrow("URL") << out.context().url << "\n";
        out.kvrow("Client HTTP") << out.context().version << "</td></tr>\n";
        for(auto& hv : out.context().rx_headers)
            out.kvrow(hv.first) << hv.second;
    }
    
    void    reg_dev_hello()
    {
        reg_webpage<page_dev_hello>("/dev/hello");
    }
}
