////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "PlaceHtml.hpp"
#include <yq/mithril/atom/AtomCDB.hpp>
#include <yq/mithril/atom/AtomHtml.hpp>
#include <yq/mithril/place/PlaceCDB.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&h, Place v)
    {
        //  more TODO...
        h << cdb::label(cdb::atom(v));  // FIXME/CRUTCH
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, Dev<Place> v)
    {
        if(v.data)
            h << "<a href=\"/dev/place?id=" << v.data.id << "\">";
        h << cdb::label(cdb::atom(v.data));  // FIXME/CRUTCH
        if(v.data)
            h << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Place> v)
    {
        if(v.data)
            h << "<a href=\"/dev/place?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }
    
    void        dev_table(WebHtml&h, const PlaceVector& places)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Brief</th></tr>\n";
        for(Place bk : places){
            Atom a = cdb::atom(bk);  // temporary hack
            auto i = cdb::info(a);
            h << "<tr><td>" << dev_id(a) << "</td><td>" << i.key << "</td><td>" 
                << i.name << "</td><td>" << i.brief << "</td></tr>\n";
        }
    }
}
