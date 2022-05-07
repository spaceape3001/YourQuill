////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "common.hpp"

#include <yq/app/DelayInit.hpp>
#include <yq/io/file_utils.hpp>
#include <yq/net/Http.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/web/VarTemplate.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebVariable.hpp>
#include <yq/wksp/Workspace.hpp>

using namespace yq;

namespace {
    void    var_abbr(WebHtml&h)
    {
        html_escape_write(h, wksp::abbreviation());
    }

    void    var_author(WebHtml&h)
    {
        html_escape_write(h, wksp::author());
    }

    void    var_body(WebHtml& h)
    {
        h << h.context().var_body;
    }

    void    var_can_edit(WebHtml&h)
    {
        //  TODO
        h << true;
    }

    void    var_home(WebHtml&h)
    {
        html_escape_write(h, wksp::home());
    }


    void    var_host(WebHtml&h)
    {
        html_escape_write(h, wksp::host());
    }

    void    var_local(WebHtml& h)
    {
        h << h.context().is_local();
    }


    void    var_logged_in(WebHtml& h)
    {
        //  TODO
        h << true;
    }

    void    var_name(WebHtml& h)
    {
        html_escape_write(h, wksp::name());
    }

    void    var_port(WebHtml&h)
    {
        h << wksp::port();
    }


    void    add_script(WebHtml& str, std::string_view name)
    {
        switch(mimeTypeForExt(file_extension(name))){
        case ContentType::javascript:
            str << "\t\t<script srce=\"/js/" << name << "\">\n";
            break;
        default:
            //  do nothing if unclear...
            break;
        }
    }
    
    void    var_scripts(WebHtml& h)
    {
        WebContext& ctx = h.context();
        for(std::string_view s : ctx.page->scripts())
            add_script(h, s);
        for(const std::string& s : ctx.var_scripts)
            add_script(h, s);
    }

    void    var_ssid(WebHtml& h)
    {
        h << h.context().session.ssid;
    }


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

    void    var_time(WebHtml& h)
    {
        h  << h.context().timestamp;
    }
    
    void    var_title(WebHtml&h)
    {
        html_escape_write(h, h.context().var_title);
    }


    void    var_user(WebHtml& h)
    {
        auto& ctx = h.context().session;
        if(!ctx.logged_in)
            h << "(anonymous)";
        else
            h << ctx.username;
    }

    void    var_year(WebHtml& h)
    {
        h << (h.context().timeparts.tm_year+1900);
    }

    YQ_INVOKE( 
        reg_webvar<var_abbr>("abbr");
        reg_webvar<var_author>("author"); 
        reg_webvar<var_body>("body");
        reg_webvar<var_can_edit>("can_edit").todo();        
        reg_webvar("footer", wksp::shared("std/footer"sv)).source(".footer");
        //reg_webvar<var_footer>("footer");
        reg_webvar<var_home>("home");        
        reg_webvar<var_host>("host");
        reg_webvar<var_local>("local");
        reg_webvar<var_logged_in>("logged_in").todo(); 
        reg_webvar<var_name>("name");
        reg_webvar<var_port>("port");
        reg_webvar<var_scripts>("scripts");
        reg_webvar<var_ssid>("ssid");
        reg_webvar("summary", wksp::shared("std/footer"sv)).source(".summary");
        //reg_webvar<var_summary>("summary"); 
        reg_webvar<var_tabbar>("tabbar");
        reg_webvar<var_time>("time");
        reg_webvar<var_title>("title");
        reg_webvar<var_user>("user");
        reg_webvar<var_year>("year"); 
    )
}

