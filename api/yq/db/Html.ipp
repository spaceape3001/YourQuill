////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Html.hpp"
#include <yq/io/Execute.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/wksp/Workspace.hpp>

#include <yq/log/Logging.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml&h, const Escape&v)
        {
            html_escape_write(h, v.text);
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const FormStart&v)
        {
            bool    inspect = v.force_inspect || h.context().session.inspect_submit;
            h << "<form action=\"";
            if(inspect){
                Url u2  = v.url;
                u2.path = "/dev/echo";
                h << u2;
            } else {
                h << v.url;
            }
            h << "\" method=\"post\">\n";
            if(inspect)
                h << "<input type=\"hidden\" name=\"url\" id=\"url\" value=\"" << v.url << "\">\n";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const ICheck&v)
        {
            h << "<input type=\"checkbox\" id=\"" << v.key << "\" name=\"" << v.key << "\" value=\"true\"";
            if(v.checked)
                h << " checked=\"true\"";
            h << "><label for=\"" << v.key << "\">" << v.label << "</label>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, IEditReq)
        {
            bool    de  = h.context().session.auto_edit;
            h << "Edit Behavior:<br>";
            h << "<input type=\"radio\" name=\"edit\" id=\"later\" value=\"later\"><label for=\"later\">";
            if(!de)
                h << "<b><u>";
            h << "Later";
            if(!de)
                h << "</u></b>";
            h << "</label></br>\n";
            h << "<input type=\"radio\" name=\"edit\" id=\"now\" value=\"now\"><label for=\"now\">";
            if(de)
                h << "<b><u>";
            h << "Now";
            if(de)
                h << "</u></b>";
            h << "</label></br>\n";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const IHidden&v)
        {
            h << "<input type=\"hidden\" id=\"" << v.key << "\" name=\"" << v.key << "\" value=\"";
            html_escape_write(h, v.value);
            h  << "\">";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, IKey)
        {
            h << "<input type=\"text\" name=\"key\" id=\"key\">";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const ILabel&v)
        {
            h << "<label for=\"" << v.key << "\">";
            html_escape_write(h, v.label);
            h << "</label>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const ILine&v)
        {
            h << "<input type=\"text\" name=\"" << v.key <<  "\" id=\"" << v.key << "\" value=\"";
            html_escape_write(h, v.value);
            h << "\"";
            if(v.size)
                h << " size=\"" << v.size << "\"";
            h << ">";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const ISubmit&v)
        {
            h << v.submit << v.reset;
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const IText&v)
        {
            h << "<textarea id=\"" << v.key << "\" name=\"" << v.key << "\" rows=\"" << v.rows << "\" cols=\"" 
              << v.cols << "\">";
              
            html_escape_write(h, v.value);
            
            h << "</textarea>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const Icon<std::string>&v)
        {
            h << "<img src=\"" << v.src << "\" class=\"";
            switch(h.context().session.icon_size){
            case SizeDesc::Original:
                h << "original";
                break;
            case SizeDesc::Large:
                h << "large";
                break;
            case SizeDesc::Medium:
                h << "medium";
                break;
            case SizeDesc::Small:
            default:
                h << "small";
                break;
            }
            h << "\"";
            if(!v.alt.empty()){
                h << " alt=\"";
                html_escape_write(h, v.alt);
                h << "\"";
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
