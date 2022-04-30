////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/html_common.hpp>

namespace yq {
    struct Atom;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Atom);
        WebHtml&    operator<<(WebHtml&, Dev<Atom>);
        WebHtml&    operator<<(WebHtml&, DevID<Atom>);
        void        dev_table(WebHtml&, const std::vector<Atom>&);
    }
}

