////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "EntityHtml.hpp"
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/atom/AtomHtml.hpp>
#include <mithril/entity/EntityCDB.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&h, Entity v)
    {
        //  more TODO...
        h << cdb::label(cdb::atom(v));  // FIXME/CRUTCH
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, Dev<Entity> v)
    {
        if(v.data)
            h << "<a href=\"/dev/entity?id=" << v.data.id << "\">";
        h << cdb::label(cdb::atom(v.data));  // FIXME/CRUTCH
        if(v.data)
            h << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Entity> v)
    {
        if(v.data)
            h << "<a href=\"/dev/entity?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }
    
    void        dev_table(WebHtml&h, const std::vector<Entity>& entitys)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Brief</th></tr>\n";
        for(Entity bk : entitys){
            Atom a = cdb::atom(bk);  // temporary hack
            auto i = cdb::info(a);
            h << "<tr><td>" << dev_id(a) << "</td><td>" << i.key << "</td><td>" 
                << i.name << "</td><td>" << i.brief << "</td></tr>\n";
        }
    }
}
