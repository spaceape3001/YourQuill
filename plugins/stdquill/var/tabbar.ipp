////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    var_tabbar(WebHtml& h)
    {
        WebContext& ctx = h.context();
        const WebGroup* grp = ctx.page -> group();
        if(!grp)
            return ;
    
        h << "<table class=\"tabbar\"><tr>\n";
        for(const WebPage* p : grp -> pages){
            bool    us = p == ctx.page;
            if(us) {
                h << "<td class=\"tb-select\">";
                html_escape_write(h, p -> label());
                h << "</td>";
            } else {
                UrlView url = ctx.url;
                url.path    = p->path();
                h << "<td class=\"tabbar\"><a href=\"" << url << "\">";
                html_escape_write(h, p -> label());
                h << "</a></td>";
            }
        }
        h << "</td></table>\n";
    }

    YQ_INVOKE( reg_webvar<var_tabbar>("tabbar"); )
}

