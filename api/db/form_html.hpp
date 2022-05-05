////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/enum/Submit.hpp>
#include <yq/enum/Reset.hpp>

namespace yq {
    class WebHtml;
    
    namespace html {
        WebHtml&    operator<<(WebHtml&, Reset);    
        WebHtml&    operator<<(WebHtml&, Submit);    
    }
}

