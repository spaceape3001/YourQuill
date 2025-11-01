////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>
#include <mithril/atom/Atom.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&, Atom);
    WebHtml&    operator<<(WebHtml&, Dev<Atom>);
    WebHtml&    operator<<(WebHtml&, const Dev<AtomVector>&);
    WebHtml&    operator<<(WebHtml&, const Dev<AtomSet>&);
    WebHtml&    operator<<(WebHtml&, DevID<Atom>);
    WebHtml&    operator<<(WebHtml&, DevID<Atom::Property>);
    
    void dev_table(WebHtml&, const AtomVector&);
    void dev_table(WebHtml&, const Atom::PropertyVector&);
    void dev_title(WebHtml&, Atom, std::string_view extra=std::string_view());
}

