////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryHtml.hpp"
#include <basic/TextUtils.hpp>
#include <mithril/http/file/DocumentHtml.hpp>
#include <mithril/http/file/RootHtml.hpp>
#include <mithril/http/web/WebContext.hpp>
#include <mithril/http/web/WebHtml.hpp>
#include <mithril/kernel/image/ImageCDB.hpp>
#include <mithril/kernel/org/CategoryCDB.hpp>
#include <mithril/http/HtmlLayout.hpp>

namespace yq {
    namespace html {
        WebHtml&    operator<<(WebHtml& h, Category v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(v);
            //  end the url (later)
            return h;
        }
    
        WebHtml&    operator<<(WebHtml&h, Dev<Category> v)
        {
            std::string n       = cdb::label(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/category?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Category> v)
        {
            if(v.data)
                h << "<a href=\"/dev/category?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void        admin_table(WebHtml&h, const std::vector<Category>& categories)
        {
            auto tac = h.table();
            auto iz = h.context().session.icon_size;
            html::columns(h, categories, 
                [&](Category c)
                {
                    if(c){
                        Image   i   = cdb::icon(c);
                        if(i){
                            h << cdb::thumbnail(i, iz);
                        } else
                            h << "<img src=\"/img/generic.svg\" class=\"" << iz << "\">";
                    }
                },
                [&](Category c)
                {
                    if(c){
                        h << "<a href=\"/admin/category?id=" << c.id << "\">" << cdb::label(c) << "</a>";
                    }
                }
            );
        }

        void    dev_table(WebHtml& h, const std::vector<Category>&categorys)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
            for(Category t : categorys){
                auto i  = cdb::info(t);
                h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" << i.name
                    << "</td><td>" << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }

        void        new_category_control(WebHtml&h, std::string_view npath)
        {
            Url url;
            url.path=copy(npath);
            h << html::form_start(url, false);
            h << "Add Category:<br>";
            h << ikey();
            h << "<br><hr width=\"10%\">\n";
            h << iroot( DataRole::Config, Access::ReadWrite);
            h << "<hr width=\"10%\">\n";
            h << iedit();
            h << "<hr width=\"10%\">\n";
            h << Submit(Submit::Create);
            h << "</form>\n";
        }
    }
}
