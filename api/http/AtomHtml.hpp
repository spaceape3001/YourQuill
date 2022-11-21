////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <http/Html.hpp>
#include <kernel/preamble.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&, Atom);
        WebHtml&    operator<<(WebHtml&, Dev<Atom>);
        WebHtml&    operator<<(WebHtml&, DevID<Atom>);
        void        dev_table(WebHtml&, const AtomVector&);
    }
}


