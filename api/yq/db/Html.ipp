////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Html.hpp"
#include <yq/io/Execute.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/wksp/Workspace.hpp>

#include <yq/log/Logging.hpp>

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

        WebHtml&    operator<<(WebHtml&h, const MarkdownText&data)
        {
            static const ProcessDescriptor    us[] = {
                ProcessDescriptor{{ wksp::perl().string(), wksp::markdown().string()}},
                ProcessDescriptor{{ wksp::perl().string(), wksp::smartypants().string()}}
            };
            
            ByteArray   buf(data.data.data(), data.data.size());
            for(auto& u : us){
                ByteArray   b2  = u.execute(buf);
                std::swap(b2, buf);
            }

            h << buf.as_view();
            return h;
        }
    }
}
