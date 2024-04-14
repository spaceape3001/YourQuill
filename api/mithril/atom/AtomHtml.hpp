////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/atom/Atom.hpp>

namespace yq::mithril {
    struct Atom;
    
    namespace html {
        WebHtml&    operator<<(WebHtml&, Atom);
        WebHtml&    operator<<(WebHtml&, Dev<Atom>);
        WebHtml&    operator<<(WebHtml&, const Dev<std::vector<Atom>>&);
        WebHtml&    operator<<(WebHtml&, const Dev<std::set<Atom>>&);
        WebHtml&    operator<<(WebHtml&, DevID<Atom>);
        WebHtml&    operator<<(WebHtml&, DevID<Atom::Property>);
        void        dev_table(WebHtml&, const std::vector<Atom>&);
        void        dev_table(WebHtml&, const std::vector<Atom::Property>&);
    }
}


