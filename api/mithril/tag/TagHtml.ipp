////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagHtml.hpp"
#include <basic/TextUtils.hpp>
#include <mithril/document/DocumentHtml.hpp>
#include <mithril/root/RootHtml.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/html/HtmlLayout.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&h, Tag t)
    {
        Thumbnail th = cdb::thumbnail(cdb::icon(t), h.context().session.icon_size);
        
        //  start the url (later)
        if(th)
            h << th << " ";
        
        h << cdb::label(t);
        //  end the url (later)
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, Dev<Tag> v)
    {
        h << "<a href=\"/dev/tag?id=" << v.data.id << "\">";
        h << "[" << v.data.id << "] " << cdb::label(v.data);
        h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, DevID<Tag> v)
    {
        if(v.data)
            h << "<a href=\"/dev/tag?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    void    admin_table(WebHtml&h, const std::vector<Tag>&tags)
    {
        auto tac = h.table();
        auto iz = h.context().session.icon_size;
        html::columns(h, tags, 
            [&](Tag c)
            {
                if(c){
                    Image   i   = cdb::icon(c);
                    if(i){
                        h << cdb::thumbnail(i, iz);
                    } else
                        h << "<img src=\"/img/generic.svg\" class=\"" << iz << "\">";
                }
            },
            [&](Tag c)
            {
                if(c){
                    h << "<a href=\"/admin/tag?id=" << c.id << "\">" << cdb::label(c) << "</a>";
                }
            }
        );
    }

    void    dev_table(WebHtml& h, const std::vector<Tag>&tags)
    {
        auto ta = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Leaf</th><th>Brief</th></tr>\n";
        for(Tag t : tags){
            auto i  = cdb::info(t);
            h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" << i.name
                << "</td><td>" << dev(i.doc) << "</td><td>" << /* dev(cdb::leaf(t)) << */ "</td><td>" << i.brief << "</td></tr>\n";
        }
    }

    void        new_tag_control(WebHtml&h, std::string_view npath)
    {
        Url url;
        url.path=copy(npath);
        h << html::form_start(url, false);
        h << "Add Tag:<br>";
        h << ikey();
        h << "<br><hr width=\"10%\">\n";
        h << iroot( DataRole::Config );
        h << "<hr width=\"10%\">\n";
        h << iedit();
        h << "<hr width=\"10%\">\n";
        h << Submit(Submit::Create);
        h << "</form>\n";
    }
}
