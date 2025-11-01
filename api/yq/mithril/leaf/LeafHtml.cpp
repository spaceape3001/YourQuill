////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "LeafHtml.hpp"
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/web/WebHtml.hpp>
#include <yq/mithril/image/ImageCDB.hpp>
#include <yq/mithril/image/ImageHtml.hpp>
#include <yq/mithril/leaf/LeafCDB.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml& h, Leaf l)
    {
        Thumbnail th = cdb::thumbnail(cdb::icon(l), h.context().session.icon_size);
        
        h << "<a href=\"/wiki/view?leaf=";
        html_escape_write(h, cdb::key(l));
        h  << "\">";
        if(th.valid())
            h << th << " ";
        h << cdb::label(l);
        h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, Dev<Leaf> v)
    {
        h << "<a href=\"/dev/leaf?id=" << v.data.id << "\">";
        h << "[" << v.data.id << "] " << cdb::label(v.data);
        h << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Leaf> v)
    {
        if(v.data)
            h << "<a href=\"/dev/leaf?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    void        dev_table(WebHtml&h, const std::vector<Leaf>&leafs)
    {
        auto t=h.table();
        h << "<tr><th><ID></th><th>key</th><th>Title</th></tr>\n";
        for(Leaf l : leafs){
            auto    i   = cdb::info(l);
            h << "<tr><td>" << dev_id(l) << "</td><td><a href=\"/leaf?id=" << l.id << "\">"
                << i.key << "<a></td><td>" << i.title << "</td></tr>\n";
        }
    }

    void    dev_title(WebHtml& h, Leaf x, std::string_view extra)
    {
        auto t = h.title();
        h << "Leaf \"" << cdb::label(x) << "\" (" << x.id << " &mdash; " << cdb::key(x) << ")";
        if(!extra.empty())
            h << ": " << extra;
    }
}
