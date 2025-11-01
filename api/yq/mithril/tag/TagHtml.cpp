////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "TagHtml.hpp"
#include <yq/text/Comma.hpp>

#include <yq/mithril/bit/NKI.hpp>
#include <yq/mithril/document/DocumentHtml.hpp>
#include <yq/mithril/root/RootHtml.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/web/WebHtml.hpp>
#include <yq/mithril/image/ImageCDB.hpp>
#include <yq/mithril/image/ImageHtml.hpp>
#include <yq/mithril/tag/TagCDB.hpp>
#include <yq/mithril/html/HtmlLayout.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::html {
    WebHtml&    operator<<(WebHtml&h, Tag t)
    {
        Thumbnail th = cdb::thumbnail(cdb::icon(t), h.context().session.icon_size);
        
        //  start the url (later)
        if(th.valid())
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

    WebHtml&    operator<<(WebHtml& h, const TagSet& vdata)
    {
        Comma   comma(", ");
        for(Tag v : vdata){
            h << comma.text() << v;
            ++comma;
        }
        return h;
    }
    
    WebHtml&    operator<<(WebHtml& h, const TagVector& vdata)
    {
        Comma   comma(", ");
        for(Tag v : vdata){
            h << comma.text() << v;
            ++comma;
        }
        return h;
    }

    WebHtml&    operator<<(WebHtml& h, const Dev<TagSet>& vdata)
    {
        Comma   comma(", ");
        for(Tag v : vdata.data){
            h << comma.text() << dev(v);
            ++comma;
        }
        return h;
    }

    WebHtml&    operator<<(WebHtml& h, const Dev<TagVector>& vdata)
    {
        Comma   comma(", ");
        for(Tag v : vdata.data){
            h << comma.text() << dev(v);
            ++comma;
        }
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

    void    admin_table(WebHtml&h, const TagVector&tags)
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

    void    dev_table(WebHtml& h, const TagVector&tags)
    {
        auto ta = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Leaf</th><th>Brief</th></tr>\n";
        for(Tag t : tags){
            auto i  = cdb::info(t);
            h << "<tr><td>" << dev_id(t) << "</td><td>" << i.key << "</td><td>" << i.name
                << "</td><td>" << dev(i.doc) << "</td><td>" << /* dev(cdb::leaf(t)) << */ "</td><td>" << i.brief << "</td></tr>\n";
        }
    }

    void    dev_title(WebHtml& h, Tag x, std::string_view extra)
    {
        auto t = h.title();
        auto i = cdb::nki(x);
        h << "Tag \"" << i.name << "\" (" << x.id << " &mdash; " << i.key << ")";
        if(!extra.empty())
            h << ": " << extra;
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
