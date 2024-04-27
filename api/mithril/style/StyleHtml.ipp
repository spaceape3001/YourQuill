////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/style/StyleCDB.hpp>
#include <mithril/style/StyleHtml.hpp>
#include <mithril/style/StyleInfo.hpp>

namespace yq::mithril::html {

#if 0
    WebHtml&    operator<<(WebHtml& h, Style l)
    {
        Thumbnail th = cdb::thumbnail(cdb::icon(l), h.context().session.icon_size);
        
        h << "<a href=\"/wiki/view?style=";
        html_escape_write(h, cdb::key(l));
        h  << "\">";
        if(th)
            h << th << " ";
        h << cdb::label(l);
        h << "</a>";
        return h;
    }
#endif

    WebHtml&    operator<<(WebHtml&h, Dev<Style> v)
    {
        h << "<a href=\"/dev/style?id=" << v.data.id << "\">";
        h << "[" << v.data.id << "] " << cdb::label(v.data);
        h << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Style> v)
    {
        if(v.data)
            h << "<a href=\"/dev/style?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    void    admin_table(WebHtml&h, const StyleVector&styles)
    {
        auto tac = h.table();
        auto iz = h.context().session.icon_size;
        html::columns(h, styles, 
            [&](Style c)
            {
                if(c){
                    Image   i   = cdb::icon(c);
                    if(i){
                        h << cdb::thumbnail(i, iz);
                    } else
                        h << "<img src=\"/img/generic.svg\" class=\"" << iz << "\">";
                }
            },
            [&](Style c)
            {
                if(c){
                    h << "<a href=\"/admin/style?id=" << c.id << "\">" << cdb::label(c) << "</a>";
                }
            }
        );
    }


    void        dev_table(WebHtml&h, const StyleVector&styles)
    {
        auto t=h.table();
        h << "<tr><th><ID></th><th>key</th><th>Name</th></tr>\n";
        for(Style l : styles){
            auto    i   = cdb::info(l);
            h << "<tr><td>" << dev_id(l) << "</td><td><a href=\"/style?id=" << l.id << "\">"
                << i.key << "<a></td><td>" << i.name << "</td></tr>\n";
        }
    }

    void        new_style_control(WebHtml&h, std::string_view npath)
    {
        Url url;
        url.path=copy(npath);
        h << html::form_start(url, false);
        h << "Add Style:<br>";
        h << ikey();
        h << "<br><hr width=\"10%\">\n";
        h << iroot( DataRole::Config );
        h << "<hr width=\"10%\">\n";
        h << iedit();
        h << "<hr width=\"10%\">\n";
        h << Submit(Submit::Create);
        h << "</form>\n";
    }
}
