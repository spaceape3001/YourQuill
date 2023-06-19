////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/html/Html.hpp>

namespace yq::mithril {
    struct Game;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Game);
        WebHtml&    operator<<(WebHtml&, Dev<Game>);
        WebHtml&    operator<<(WebHtml&, DevID<Game>);
        void        dev_table(WebHtml&, const std::vector<Game>&);
        
    }
}
