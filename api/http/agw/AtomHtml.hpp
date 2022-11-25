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
        WebHtml&    operator<<(WebHtml&, agw::Atom);
        WebHtml&    operator<<(WebHtml&, Dev<agw::Atom>);
        WebHtml&    operator<<(WebHtml&, DevID<agw::Atom>);
        void        dev_table(WebHtml&, const agw::AtomVector&);
    }
}


