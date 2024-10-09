////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DocumentHtml.hpp"
#include <yq-toolbox/io/DirUtils.hpp>
#include <yq-toolbox/text/match.hpp>
#include <yq/typedef/string.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/wksp/Workspace.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Document);

    WebHtml&    operator<<(WebHtml&h, Dev<Document> v)
    {
        std::string n       = cdb::name(v.data);
        if(n.empty())
            n   = "(no-name)";
        if(v.data)
            h << "<a href=\"/dev/document?id=" << v.data.id << "\">";
        h << "[" << v.data.id << "] " << n;
        if(v.data)
            h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, DevID<Document> v)
    {
        if(v.data)
            h << "<a href=\"/dev/document?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, const Icon<Document>& v)
    {
        std::string a   = v.alt;
        if(a.empty())
            a   = cdb::key(v.src);
        
        Image   i   = cdb::icon(v.src);
        if(i){
            h << cdb::thumbnail(i, a);
        } else {
            Icon<std::string>   fwd;
            fwd.src = "/img/document.svg";
            fwd.style = v.style;
            fwd.alt = std::move(a);
            h << fwd;
        }
        return h;
    }

    void    dev_table(WebHtml& h, const DocumentVector& documents)
    {
        auto _tab = h.table();
        h << "<tr><th>ID</th><th>Fragments</th><th>Key</th><th>Name</th><th>Suffix</th>\n";
        for(Document a : documents){
            auto i = cdb::info(a);
            h << "<tr><td>" << dev_id(a)  << "</td><td>" << cdb::fragments_count(a) 
              << "</td><td>" << i.key << "</td><td>" << i.name << "</td><td>" << i.suffix << "</td></tr>\n";
        }
    }

    void    dev_title(WebHtml& h, Document x, std::string_view extra)
    {
        auto t = h.title();
        h.title() << "Document (" << x.id << " &mdash; " << cdb::key(x) << ")";
        if(!extra.empty())
            h << ": " << extra;
    }

    string_map_t    make_file_extension_icons()
    {
        string_map_t    ret;
        
        dir::for_all_children(wksp::shared_all("www/img/ext"sv), dir::NO_DIRS, [&](const std::filesystem::path& p){
            auto sfx = p.extension();
            if(!is_similar(sfx.c_str(), ".svg"))
                return;
            auto file           = p.filename();
            auto key            = p.stem();
            ret[key.c_str()]    = "/img/ext/"s + file.c_str();
        });
        
        return ret;
    }
    
    const string_map_t& file_extension_icons()
    {
        static string_map_t     s_data  = make_file_extension_icons();
        return s_data;
    }

}
