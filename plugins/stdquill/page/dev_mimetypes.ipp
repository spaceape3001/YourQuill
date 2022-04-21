////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void page_dev_mimetypes(WebHtml&h)
    {
        h.title("Mime Types");
        auto ta = h.table();
        h << "<tr><th>ID</th><th>KEY</th><th>MIME-TYPE</th></tr>\n";
        for(ContentType ct : ContentType::all_values())
            h << "<tr><td>" << ct.value() << "</td><td>" << ct.key() << "</td><td>" << mimeType(ct) << "</td></tr>\n";
    }
    
    YQ_INVOKE( reg_webpage<page_dev_mimetypes>("/dev/mimetypes"); )
    
}
