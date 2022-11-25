////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassHtml.hpp"
#include <basic/StreamOps.hpp>
#include <basic/TextUtils.hpp>
#include <basic/stream/Text.hpp>
#include <http/atom/ClassArg.hpp>
#include <http/file/DocumentHtml.hpp>
#include <http/file/RootHtml.hpp>
#include <http/web/WebContext.hpp>
#include <http/web/WebHtml.hpp>
#include <kernel/atom/Class.hpp>
#include <kernel/atom/ClassCDB.hpp>
#include <kernel/file/Root.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <http/image/ImageHtml.hpp>
#include <http/HtmlLayout.hpp>

namespace yq {


    namespace html {
        WebHtml&    operator<<(WebHtml&h, Class v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            if(v)
                h << "<a href=\"/class?key=" << cdb::key(v) << "\">";
            //  start the url (later)
            if(th)
                h << th << " ";
            h << cdb::label(v);
            if(v)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Class> v)
        {
            if(v.data)
                h << "<a href=\"/dev/class?id=" << v.data.id << "\">";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Class> v)
        {
            if(v.data)
                h << "<a href=\"/dev/class?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const Edit<Class>& v)
        {
            Url             url;
            url.path        = "/admin/class/edit";
            const Root* rt  = v.root;
            if(!rt)
                rt          = h.context().def_root(DataRole::Config);

            Class   c   = v.value;
            if(!c)
                c       = arg::class_(h);
            
            {
                stream::Text    args(url.query);
                if(c)
                    args << "id=" << c.id;
                if(rt){
                    if(c)
                        args << '&';
                    args << "root=" << rt->id;
                }
            }
            
            h << form_start(url, v.force_inspect);
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, const Plural<Class>& v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v.data), h.context().session.icon_size);
            if(v.data)
                h << "<a href=\"/atoms?class=" << cdb::key(v.data) << "\">";
            //  start the url (later)
            if(th)
                h << th << " ";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }

        void        admin_table(WebHtml&h, const std::vector<Class>&classes)
        {
            auto tac = h.table();
            auto iz = h.context().session.icon_size;
            html::columns(h, classes, 
                [&](Class c)
                {
                    if(c){
                        Image   i   = cdb::icon(c);
                        if(i){
                            h << cdb::thumbnail(i, iz);
                        } else
                            h << "<img src=\"/img/generic.svg\" class=\"" << iz << "\">";
                    }
                },
                [&](Class c)
                {
                    if(c){
                        h << "<a href=\"/admin/class?id=" << c.id << "\">" << cdb::label(c) << "</a>";
                    }
                }
            );
        }
        

        void        dev_table(WebHtml&h, const std::vector<Class>& classes)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
            for(Class c : classes){
                auto i = cdb::info(c);
                h << "<tr><td>" << dev_id(c) << "</td><td>" << i.key << "</td><td>" << i.name << "</td><td>" 
                    << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }

        void        dev_table(WebHtml& h, const std::vector<Class::Rank>&data, std::string_view rankName)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>" << rankName << "</th></tr>\n";
            for(const Class::Rank& cr : data){
                auto i = cdb::info(cr.cls);
                h << "<tr><td>" << dev_id(cr.cls) << "</td><td>" << i.key << "</td><td>" << i.name 
                    << "</td><td>" << cr.rank << "</td></tr>\n";
            }
        }

        void        new_class_control(WebHtml&h, std::string_view npath)
        {
            Url url;
            url.path=copy(npath);
            h << html::form_start(url, false);
            h << "Add Class:<br>";
            h << ikey();
            h << "<br><hr width=\"10%\">\n";
            h << iroot( DataRole::Config, Access::ReadWrite );
            h << "<hr width=\"10%\">\n";
            h << iedit();
            h << "<hr width=\"10%\">\n";
            h << Submit(Submit::Create);
            h << "</form>\n";
        }
    }
}
