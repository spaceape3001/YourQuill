////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FieldHtml.hpp"
#include <yq/text/Comma.hpp>
#include <yq/text/format.hpp>
//
#include <yq/core/StreamOps.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/class/ClassHtml.hpp>
#include <mithril/root/RootHtml.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/field/FieldInfo.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/image/ImageHtml.hpp>
#include <mithril/html/HtmlLayout.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&h, Field v)
    {
        Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
        
        //  start the url (later)
        if(th.valid())
            h << th << " ";
        
        h << cdb::label(v);
        //  end the url (later)
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, Dev<Field> v)
    {
        if(v.data)
            h << "<a href=\"/dev/field?id=" << v.data.id << "\">";
        h << cdb::label(v.data);
        if(v.data)
            h << "</a>";
        return h;
    }

    WebHtml&    operator<<(WebHtml&h, const std::set<Field>&vdata)
    {
        Comma   comma(", ");
        for(Field v : vdata){
            h << comma.text() << v;
            ++comma;
        }
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, const FieldVector&vdata)
    {
        Comma   comma(", ");
        for(Field v : vdata){
            h << comma.text() << v;
            ++comma;
        }
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, const Dev<std::set<Field>>&vdata)
    {
        Comma   comma(", ");
        for(Field v : vdata.data){
            h << comma.text() << dev(v);
            ++comma;
        }
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, const Dev<FieldVector>&vdata)
    {
        Comma   comma(", ");
        for(Field v : vdata.data){
            h << comma.text() << dev(v);
            ++comma;
        }
        return h;
    }
    
    WebHtml&    operator<<(WebHtml&h, DevID<Field> v)
    {
        if(v.data)
            h << "<a href=\"/dev/field?id=" << v.data.id << "\">";
        h << v.data.id;
        if(v.data)
            h << "</a>";
        return h;
    }


    void        admin_table(WebHtml&h, const FieldVector&fields)
    {
        auto tac = h.table();
        auto iz = h.context().session.icon_size;
        html::columns(h, fields, 
            [&](Field c)
            {
                if(c){
                    Image   i   = cdb::icon(c);
                    if(i){
                        h << cdb::thumbnail(i, iz);
                    } else
                        h << "<img src=\"/img/generic.svg\" class=\"" << iz << "\">";
                }
            },
            [&](Field c)
            {
                if(c){
                    h << "<a href=\"/admin/class?id=" << c.id << "\">" << cdb::label(c) << "</a>";
                }
            }
        );
    }
    
    void      dev_table(WebHtml&h, const FieldVector& fields)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Class</th><th>Key</th><th>Name</th><th>Brief</th>\n";
        for(Field f : fields){
            auto i = cdb::info(f);
            h << "<tr><td>" << dev_id(f) << "</td><td>" << dev(i.class_) << "</td><td>" 
                << i.key <<"</td><td>" << i.name << "</td><td>"
                << i.brief <<"</td></tr>\n";
        }
    }
    
    void    dev_title(WebHtml& h, Field x, std::string_view extra)
    {
        auto t = h.title();
        auto i = cdb::nki(x);
        h << "Field \"" << i.name << "\" (" << x.id << " &mdash; " << i.key << ")";
        if(!extra.empty())
            h << ": " << extra;
    }

    void     new_field_control(WebHtml&h, std::string_view npath, Class c)
    {
        Url url;
        url.path=copy(npath);
        h << html::form_start(url, false);
        h << "Add Field:<br>";
        h << ikey();
        h << "<br><hr width=\"10%\">\n";
        h << iroot( DataRole::Config, Access::ReadWrite );
        h << "<hr width=\"10%\">\n";
        h << iedit();
        h << "<hr width=\"10%\">\n";
        h << Submit(Submit::Create);
        if(c){
            h << ihidden("class", to_string(c.id));
        }
        
        h << "</form>\n";
    }

}
