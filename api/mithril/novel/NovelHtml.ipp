////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "NovelHtml.hpp"
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/novel/NovelCDB.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml& h, Novel l)
    {
        Thumbnail th = cdb::thumbnail(cdb::icon(l), h.context().session.icon_size);
        
        h << "<a href=\"/wiki/view?novel=";
        html_escape_write(h, cdb::key(l));
        h  << "\">";
        if(th)
            h << th << " ";
        h << cdb::label(l);
        h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, Dev<Novel> v)
    {
        h << "<a href=\"/dev/novel?id=" << v.data.id << "\">";
        h << "[" << v.data.id << "] " << cdb::label(v.data);
        h << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Novel> v)
    {
        if(v.data)
            h << "<a href=\"/dev/novel?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    void        dev_table(WebHtml&h, const std::vector<Novel>&novels)
    {
        auto t=h.table();
        h << "<tr><th><ID></th><th>key</th><th>Title</th></tr>\n";
        for(Novel l : novels){
            auto    i   = cdb::info(l);
            h << "<tr><td>" << dev_id(l) << "</td><td><a href=\"/novel?id=" << l.id << "\">"
                << i.key << "<a></td><td>" << i.title << "</td></tr>\n";
        }
    }

    void    dev_title(WebHtml& h, Novel x, std::string_view extra)
    {
        auto t = h.title();
        h << "Novel \"" << cdb::label(x) << "\" (" << x.id << " &mdash; " << cdb::key(x) << ")";
        if(!extra.empty())
            h << ": " << extra;
    }
}
