////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void p_dev_mimetypes(WebHtml&h)
    {
        h.title("Mime Types");
        auto ta = h.table();
        h << "<tr><th>ID</th><th>KEY</th><th>MIME-TYPE</th></tr>\n";
        for(ContentType ct : ContentType::all_values())
            h << "<tr><td>" << ct.value() << "</td><td>" << ct.key() << "</td><td>" << mimeType(ct) << "</td></tr>\n";
    }

    void    reg_dev_mimetypes()
    {
        reg_webpage<p_dev_mimetypes>("/dev/mimetypes");
    }
}
