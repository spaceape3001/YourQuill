////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventHtml.hpp"
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/atom/AtomHtml.hpp>
#include <mithril/event/EventCDB.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&h, Event v)
    {
        //  more TODO...
        h << cdb::label(cdb::atom(v));  // FIXME/CRUTCH
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, Dev<Event> v)
    {
        if(v.data)
            h << "<a href=\"/dev/event?id=" << v.data.id << "\">";
        h << cdb::label(cdb::atom(v.data));  // FIXME/CRUTCH
        if(v.data)
            h << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Event> v)
    {
        if(v.data)
            h << "<a href=\"/dev/event?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }
    
    void        dev_table(WebHtml&h, const std::vector<Event>& events)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Brief</th></tr>\n";
        for(Event ev : events){
            Atom a = cdb::atom(ev);  // temporary hack
            auto i = cdb::info(a);
            h << "<tr><td>" << dev_id(a) << "</td><td>" << i.key << "</td><td>" << i.brief << "</td></tr>\n";
        }
    }
}
