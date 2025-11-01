////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>

namespace yq::mithril {
    struct Graphviz;

    namespace html {
        WebHtml&    operator<<(WebHtml&, Graphviz);
        WebHtml&    operator<<(WebHtml&, Dev<Graphviz>);
        WebHtml&    operator<<(WebHtml&, DevID<Graphviz>);
        void dev_table(WebHtml&, const std::vector<Graphviz>&);
        void dev_title(WebHtml&, Graphviz x, std::string_view extra=std::string_view());
    }
}


