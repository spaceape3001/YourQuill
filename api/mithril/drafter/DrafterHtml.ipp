////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/drafter/DrafterCDB.hpp>
#include <mithril/drafter/DrafterHtml.hpp>
#include <mithril/drafter/DrafterInfo.hpp>

namespace yq::mithril::html {

#if 0
    WebHtml&    operator<<(WebHtml& h, Drafter l)
    {
        Thumbnail th = cdb::thumbnail(cdb::icon(l), h.context().session.icon_size);
        
        h << "<a href=\"/wiki/view?drafter=";
        html_escape_write(h, cdb::key(l));
        h  << "\">";
        if(th)
            h << th << " ";
        h << cdb::label(l);
        h << "</a>";
        return h;
    }
#endif

    WebHtml&    operator<<(WebHtml&h, Dev<Drafter> v)
    {
        h << "<a href=\"/dev/drafter?id=" << v.data.id << "\">";
        h << "[" << v.data.id << "] " << cdb::label(v.data);
        h << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Drafter> v)
    {
        if(v.data)
            h << "<a href=\"/dev/drafter?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    void        dev_table(WebHtml&h, const DrafterVector&drafters)
    {
        auto t=h.table();
        h << "<tr><th><ID></th><th>key</th><th>Name</th></tr>\n";
        for(Drafter l : drafters){
            auto    i   = cdb::info(l);
            h << "<tr><td>" << dev_id(l) << "</td><td><a href=\"/drafter?id=" << l.id << "\">"
                << i.key << "<a></td><td>" << i.name << "</td></tr>\n";
        }
    }

    void    dev_title(WebHtml& h, Drafter x, std::string_view extra)
    {
        auto t = h.title();
        auto i = cdb::nki(x);
        h << "Drafter \"" << i.name << "\" (" << x.id << " &mdash; " << i.key << ")";
        if(!extra.empty())
            h << ": " << extra;
    }

}
