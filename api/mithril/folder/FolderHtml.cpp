////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FolderHtml.hpp"
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/folder/FolderInfo.hpp>
#include <mithril/image/ImageCDB.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Folder);

    WebHtml&    operator<<(WebHtml&h, Dev<Folder> v)
    {
        std::string n       = cdb::name(v.data);
        if(n.empty())
            n   = "(no-name)";
        if(v.data)
            h << "<a href=\"/dev/folder?id=" << v.data.id << "\">";
        h << "[" << v.data.id << "] " << n;
        if(v.data)
            h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, DevID<Folder> v)
    {
        if(v.data)
            h << "<a href=\"/dev/folder?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, const Icon<Folder>&v)
    {
        std::string a   = v.alt;
        if(a.empty())
            a   = cdb::key(v.src);
        
        Image   i   = cdb::icon(v.src);
        if(i){
            h << cdb::thumbnail(i, a);
        } else {
            Icon<std::string>   fwd;
            fwd.src = "/img/folder.svg";
            fwd.style = v.style;
            fwd.alt = std::move(a);
            h << fwd;
        }
        return h;
    }

    void    dev_table(WebHtml& h, const FolderVector&folders)
    {
        auto _tab = h.table();
        h << "<tr><th><ID></th><th>key</th><th>Name</th><th>Documents</th><th>Children</th><th>Brief</th></tr>\n";
        for(Folder f : folders){
            auto i = cdb::info(f);
            h << "<tr><td>" << dev_id(f) << "</td><td>" << i.key
              << "</td><td>" << i.name << "</td><td>" << cdb::child_documents_count(f) << "</td><td>" << cdb::child_folders_count(f) 
              << "</td><td>" << i.brief << "</td</tr>\n";
        }
    }

    void    dev_title(WebHtml& h, Folder x, std::string_view extra)
    {
        auto t = h.title();
        h << "Folder (" << x.id << " &mdash; " << cdb::key(x) << ")";
        if(!extra.empty())
            h << ": " << extra;
    }

}
