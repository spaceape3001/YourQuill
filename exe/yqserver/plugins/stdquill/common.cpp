#include "stdquill.hpp"

WebHtml&    operator<<(WebHtml& h, SubmitLabel v)
{
    h << "<input type=\"submit\"";
    switch(v){
    case SubmitLabel::Save:
        h << " value=\"Save\"";
        break;
    case SubmitLabel::Create:
        h << " value=\"Create\"";
        break;
    default:
        break;
    }
    
    h << "><input type=\"reset\"";
    // TODO the reset
    h << ">";
    return h;
}

WebAutoClose    form(WebHtml&h, std::string_view action, bool inspect)
{
    return form(h, action, hPost, inspect);
}

WebAutoClose    form(WebHtml&h, std::string_view action, HttpOp m, bool inspect)
{
    inspect = inspect || h.context().session.inspect_submit;
    if(inspect){
        size_t  qm   = action.find_first_of('?');
        if(qm != std::string_view::npos){
            std::string_view    q   = action.substr(qm+1);
            h << "<form action=\"/dev/echo?" << q << "\" method=\"" << m.key() << "\">\n";
        } else
            h << "<form action=\"/dev/echo\" method=\"" << m.key() << "\">\n";
        h << "<input type=\"hidden\" name=\"url\" id=\"url\" value=\"" << action << "\">\n";
    } else {
        h << "<form action=\"" << action << "\" method=\"" << m.key() << "\">\n";
    }
    
    return WebAutoClose(h, "</form>");
}

#if 0

WebHtml&    operator<<(WebHtml&h, input::key)
{
    
}

WebHtml&    operator<<(WebHtml&h, input::edit_req);
WebHtml&    operator<<(WebHtml&h, input::root);
WebHtml&    operator<<(WebHtml&h, const input::check&);
WebHtml&    operator<<(WebHtml&h, const input::hidden&);
WebHtml&    operator<<(WebHtml&h, const input::label&);
WebHtml&    operator<<(WebHtml&h, const input::line&);
WebHtml&    operator<<(WebHtml&h, const input::text&);
WebHtml&    operator<<(WebHtml&h, input::control_root);
#endif
