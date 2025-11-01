////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "DirectoryHtml.hpp"
#include <mithril/directory/DirectoryCDB.hpp>
#include <mithril/root/Root.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/root/RootDir.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Directory);

    WebHtml&    operator<<(WebHtml&h, Dev<Directory> v)
    {
        std::string n       = cdb::name(v.data);
        if(n.empty())
            n   = "(no-name)";
        if(v.data)
            h << "<a href=\"/dev/directory?id=" << v.data.id << "\">";
        h << "[" << v.data.id << "] " << n;
        if(v.data)
            h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, DevID<Directory> v)
    {
        if(v.data)
            h << "<a href=\"/dev/directory?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    void    dev_table(WebHtml& h, const DirectoryVector& dirs)
    {
        auto _tab = h.table();
            h << "<tr><th>ID</th><th>Fragments</th><th>Children</th><th>Path</th></tr>\n";
        for(Directory d : dirs){
            h << "<tr><td>" << dev_id(d) << "</td><td>" 
                << cdb::child_fragments_count(d) << "</td><td>" << cdb::child_directories_count(d) << "</td><td>" << cdb::path(d) << "</td></tr>\n";
        }
    }


    void    dev_title(WebHtml& h, Directory x, std::string_view extra)
    {
        auto t = h.title();
        const RootDir* rt  = cdb::root_dir(x);
        
        h << "Directory ";
        if(rt)
            h << '[' << rt->key << ']';
        h << '(' << x.id << " &mdash; " << cdb::key(x) << ')';
        if(!extra.empty()){
            h << ": " << extra;
        }
    }

}
