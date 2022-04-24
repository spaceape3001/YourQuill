////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "struct.hpp"

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml& h, Thumbnail t)
        {
            std::string n   = cdb::label(cdb::document(t.img));
            h << "<img src=\"/thumbnail?id=" << t.img.id;
            if(t.size != SizeDesc())
                h << "&size=" << t.size.key();
            h << "\" alt=\"Thumbnail for '";
            html_escape_write(h, n);
            h << "'\" />";
            return h;
        }
    }
}


