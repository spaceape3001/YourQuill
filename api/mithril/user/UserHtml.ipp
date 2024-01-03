////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UserHtml.hpp"
#include <0/basic/TextUtils.hpp>
#include <mithril/document/DocumentHtml.hpp>
#include <mithril/root/RootHtml.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/user/UserCDB.hpp>
#include <mithril/html/HtmlLayout.hpp>

namespace yq::mithril::html {

    WebHtml&    operator<<(WebHtml&h, User v)
    {
        Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
        
        //  start the url (later)
        if(th)
            h << th << " ";
        
        h << cdb::label(v);
        //  end the url (later)
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, Dev<User> v)
    {
        std::string n       = cdb::name(v.data);
        if(n.empty())
            n   = "(no-name)";
        if(v.data)
            h << "<a href=\"/dev/user?id=" << v.data.id << "\">";
        h << "[" << v.data.id << "] " << n;
        if(v.data)
            h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, DevID<User> v)
    {
        if(v.data)
            h << "<a href=\"/dev/user?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }

    void    admin_table(WebHtml&h, const std::vector<User>&users)
    {
        auto tac = h.table();
        auto iz = h.context().session.icon_size;
        html::columns(h, users, 
            [&](User c)
            {
                if(c){
                    Image   i   = cdb::icon(c);
                    if(i){
                        h << cdb::thumbnail(i, iz);
                    } else
                        h << "<img src=\"/img/generic.svg\" class=\"" << iz << "\">";
                }
            },
            [&](User c)
            {
                if(c){
                    h << "<a href=\"/admin/user?id=" << c.id << "\">" << cdb::label(c) << "</a>";
                }
            }
        );
    }

    void    dev_table(WebHtml& h, const std::vector<User>&users)
    {
        auto ta = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Owner</th><th>Admin</th><th>Writer</th><th>Reader</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
        for(User t : users){
            auto i  = cdb::info(t);
            h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" 
                << i.is_owner << "</td><td>"
                << i.is_admin << "</td><td>"
                << i.is_writer << "</td><td>"
                << i.is_reader << "</td><td>"
                <<  i.name << "</td><td>" << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
        }
    }    

    void        new_user_control(WebHtml&h, std::string_view npath)
    {
        Url url;
        url.path=copy(npath);
        h << html::form_start(url, false);
        h << "Add User:<br>";
        h << ikey();
        h << "<br><hr width=\"10%\">\n";
        h << iroot( DataRole::Users );
        h << "<hr width=\"10%\">\n";
        h << iedit();
        h << "<hr width=\"10%\">\n";
        h << Submit(Submit::Create);
        h << "</form>\n";
    }

}
