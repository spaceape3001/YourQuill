////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/bit/NKI.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/web/WebHtml.hpp>
#include <yq/mithril/doodle/DoodleCDB.hpp>
#include <yq/mithril/doodle/DoodleHtml.hpp>
#include <yq/mithril/doodle/DoodleInfo.hpp>

namespace yq::mithril::html {

#if 0
    WebHtml&    operator<<(WebHtml& h, Doodle l)
    {
        Thumbnail th = cdb::thumbnail(cdb::icon(l), h.context().session.icon_size);
        
        h << "<a href=\"/wiki/view?doodle=";
        html_escape_write(h, cdb::key(l));
        h  << "\">";
        if(th)
            h << th << " ";
        h << cdb::label(l);
        h << "</a>";
        return h;
    }
#endif

    WebHtml&    operator<<(WebHtml&h, Dev<Doodle> v)
    {
        h << "<a href=\"/dev/doodle?id=" << v.data.id << "\">";
        h << "[" << v.data.id << "] " << cdb::label(v.data);
        h << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Doodle> v)
    {
        if(v.data)
            h << "<a href=\"/dev/doodle?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    void        dev_table(WebHtml&h, const DoodleVector&doodles)
    {
        auto t=h.table();
        h << "<tr><th><ID></th><th>key</th><th>Name</th></tr>\n";
        for(Doodle l : doodles){
            auto i   = cdb::nki(l);
            h << "<tr><td>" << dev_id(l) << "</td><td><a href=\"/doodle?id=" << l.id << "\">"
                << i.key << "<a></td><td>" << i.name << "</td></tr>\n";
        }
    }

    void    dev_title(WebHtml& h, Doodle x, std::string_view extra)
    {
        auto t = h.title();
        auto i = cdb::nki(x);
        h << "Doodle \"" << i.name << "\" (" << x.id << " &mdash; " << i.key << ")";
        if(!extra.empty())
            h << ": " << extra;
    }

}
