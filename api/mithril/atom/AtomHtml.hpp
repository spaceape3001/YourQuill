////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>

namespace yq::mithril {
    struct Atom;
    
    namespace html {
        WebHtml&    operator<<(WebHtml&, Atom);
        WebHtml&    operator<<(WebHtml&, Dev<Atom>);
        WebHtml&    operator<<(WebHtml&, DevID<Atom>);
        void        dev_table(WebHtml&, const std::vector<Atom>&);
    }
}


