////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    var_tabbar(Stream& str, WebContext& ctx)
    {
        const WebGroup* grp = ctx.page -> group();
        if(!grp)
            return ;
    
        str << "<table class=\"tabbar\"><tr>\n";
        for(const WebPage* p : grp -> pages){
            bool    us = p == ctx.page;
            if(us) {
                str << "<td class=\"tb-select\">";
                html_escape_write(str, p -> label());
                str << "</td>";
            } else {
                UrlView url = ctx.url;
                url.path    = p->path();
                str << "<td class=\"tabbar\"><a href=\"" << url << "\">";
                html_escape_write(str, p -> label());
                str << "</a></td>";
            }
        }
        str << "</td></table>\n";
    }

    YQ_INVOKE( reg_webvar<var_tabbar>("tabbar"); )
}

