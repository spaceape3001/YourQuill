////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Html.hpp"
#include <yq/stream/Ops.hpp>
#include <yq/web/WebHtml.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml& h, Reset v)
        {
            h << "<input type=\"reset\"";
            switch(v){
            case Reset::Default:
            default:
                h << " value=\"Reset\"";
                break;
            }
            h << ">";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml& h, Submit v)
        {
            h << "<input type=\"submit\"";
            switch(v){
            case Submit::Save:
                h << " value=\"Save\"";
                break;
            case Submit::Create:
                h << " value=\"Create\"";
                break;
            case Submit::Default:
            default:
                h << " value=\"Submit\"";
                break;
            }
            
            h << ">";
            return h;
        }
    }
}
