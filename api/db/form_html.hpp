////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "enum_submit.hpp"
#include "enum_reset.hpp"

namespace yq {
    class WebHtml;
    
    namespace html {
        WebHtml&    operator<<(WebHtml&, Reset);    
        WebHtml&    operator<<(WebHtml&, Submit);    
    }
}

