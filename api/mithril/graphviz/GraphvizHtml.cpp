////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "GraphvizHtml.hpp"
#include <mithril/fragment/FragmentHtml.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/graphviz/Graphviz.hpp>
#include <mithril/graphviz/GraphvizCDB.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml& h, Graphviz i)
    {
        std::string n   = cdb::name(i);
        n   = html_escape(n);
        h << "<img src=\"/graphviz/svg?id=" << i.id << "\" alt=\"" << n << "\" usemap=\"#" << n << "\">";
        h << cdb::html_text(i);
        h << '\n';
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, Dev<Graphviz>v)
    {
        h << "<a href=\"/dev/graphviz?id=" << v.data.id << "\">" << v.data.id << "</a>";
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Graphviz> v)
    {
        h << "<a href=\"/dev/graphviz?id=" << v.data.id << "\">" << v.data.id << "</a>";
        return h;
    }

    void        dev_table(WebHtml& h, const std::vector<Graphviz>& graphvizs)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Name</th><th>Dot Size</th><th>Html Size</th><th>SVG Size</th></tr>\n";
        for(Graphviz i : graphvizs){
            h << "<tr><td>" << dev_id(i) << "</td><td>" << cdb::name(i) 
                << "</td><td>"  << cdb::dot_size(i) 
                << "</td><td>"  << cdb::html_size(i) 
                << "</td><td>"  << cdb::svg_size(i) 
                << "</td></tr>\n";
        }
    }

    void    dev_title(WebHtml& h, Graphviz x, std::string_view extra)
    {
        auto t = h.title();
        h << "Graphviz \"" << cdb::name(x) << "\" (" << x.id << " &mdash; " << cdb::key(x) << ")";
        if(!extra.empty())
            h << ": " << extra;
    }

}
