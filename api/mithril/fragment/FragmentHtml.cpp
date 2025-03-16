////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FragmentHtml.hpp"
#include <mithril/fragment/FragmentCDB.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Fragment);

    WebHtml&    operator<<(WebHtml&h, Dev<Fragment> v)
    {
        std::string n       = cdb::name(v.data);
        if(n.empty())
            n   = "(no-name)";
        if(v.data)
            h << "<a href=\"/dev/fragment?id=" << v.data.id << "\">";
        h << "[" << v.data.id << "] " << n;
        if(v.data)
            h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, DevID<Fragment> v)
    {
        if(v.data)
            h << "<a href=\"/dev/fragment?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }


        void    dev_table(WebHtml& h, const FragmentVector& fragments)
        {
            auto _tab = h.table();
            h << "<tr><th>ID</th><th>Name</th><th>Size</th><th>Path</th>\n";
            for(Fragment f : fragments){
                auto i = cdb::info(f);
                h << "<tr><td>" << dev_id(f) << "</td><td>" << i.name 
                    << "</td><td>" << i.size << "</td><td>" << i.path << "</td></tr>\n";
            }
        }

        void    dev_title(WebHtml& h, Fragment x, std::string_view extra)
        {
            auto t = h.title();
            h << "Fragment (" << x.id << " &mdash; " << cdb::path(x) << ")";
            if(!extra.empty())
                h << ": " << extra;
        }
}
