////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomHtml.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/attribute/AttributeHtml.hpp>
#include <mithril/field/FieldHtml.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&h, Atom v)
    {
        //  more TODO...
        h << cdb::label(v);
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, Dev<Atom> v)
    {
        if(v.data)
            h << "<a href=\"/dev/atom?id=" << v.data.id << "\">";
        h << cdb::label(v.data);
        if(v.data)
            h << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Atom> v)
    {
        if(v.data)
            h << "<a href=\"/dev/atom?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Atom::Property> v)
    {
        if(v.data)
            h << "<a href=\"/dev/atom_property?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    void        dev_table(WebHtml&h, const std::vector<Atom>& atoms)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Brief</th></tr>\n";
        for(Atom a : atoms){
            auto i = cdb::info(a);
            h << "<tr><td>" << dev_id(a) << "</td><td>" << i.key << "</td><td>" << i.brief << "</td></tr>\n";
        }
    }

    void        dev_table(WebHtml& h, const std::vector<Atom::Property>& props)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th colspan=\"2\">Atom</th><th colspan=\"2\">Attr</th><th colspan=\"2\">Field</th><th colspan=\"2\">Source</th><th colspan=\"2\">Target</th></tr>\n";
        for(Atom::Property p : props){
            auto i = cdb::info(p);
            h   << "<tr><td>"  << p.id 
                << "</td><td>" << dev_id(i.atom)  << "</td><td>" << dev(i.atom) 
                << "</td><td>" << dev_id(i.attr)  << "</td><td>" << dev(i.attr) 
                << "</td><td>" << dev_id(i.field)  << "</td><td>" << dev(i.field) 
                << "</td><td>" << dev_id(i.source)  << "</td><td>" << dev(i.source) 
                << "</td><td>" << dev_id(i.target)  << "</td><td>" << dev(i.target) 
                << "</td></tr>\n";
        }
    }
}