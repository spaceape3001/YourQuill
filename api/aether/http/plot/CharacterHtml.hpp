////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/http/Html.hpp>

namespace yq {
    struct Character;
    namespace html {
        WebHtml&    operator<<(WebHtml&, Character);
        WebHtml&    operator<<(WebHtml&, Dev<Character>);
        WebHtml&    operator<<(WebHtml&, DevID<Character>);
        void        dev_table(WebHtml&, const std::vector<Character>&);
        
    }
}
