////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/enum/submit.hpp>
#include <yq/enum/reset.hpp>

namespace yq {
    class WebHtml;
    
    namespace html {
        WebHtml&    operator<<(WebHtml&, Reset);    
        WebHtml&    operator<<(WebHtml&, Submit);    
    }
}

