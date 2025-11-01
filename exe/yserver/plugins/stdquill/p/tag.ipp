////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/core/DelayInit.hpp>
#include <yq/mithril/image/ImageHtml.hpp>
#include <yq/mithril/root/RootDir.hpp>
#include <yq/mithril/root/RootPost.hpp>
#include <yq/mithril/tag/Tag.hpp>
#include <yq/mithril/tag/TagArg.hpp>
#include <yq/mithril/tag/TagCDB.hpp>
#include <yq/mithril/tag/TagHtml.hpp>
#include <yq/mithril/tag/TagJson.hpp>
#include <yq/mithril/tag/TagPost.hpp>
#include <yq/mithril/tag/TagSearch.hpp>
#include <yq/mithril/web/JsonAdapter.hpp>
#include <yq/mithril/wksp/Workspace.hpp>

#include "common.hpp"

using namespace yq;
using namespace yq::mithril;
using namespace yq::mithril::arg;
using namespace yq::mithril::cdb;
using namespace yq::mithril::html;

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_tag(WebContext& ctx)
        {
            Tag    v   = arg::tag(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_tag_key(WebContext& ctx)
        {
            Tag    v   = arg::tag(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_tags(WebContext& ctx)
        {
            TagVector  ret = search(ctx, TAG);
            return json{
                { "tags", json_(ret) }
            };
        }

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

        void    p_admin_tag(WebHtml& h)
        {
            Tag     t   = arg::tag(h);
            if(!t)
                throw HttpStatus::BadArgument;
                
            h.title() << "Tag (" << key(t) << ")";
            
            
            h << "TODO... tag " << label(t);
        }

        void    p_admin_tags(WebHtml& h)
        {
            h.title() << "Tags for [" << html_escape(wksp::name()) << "]";

            h << "<p><div class=\"explain\">"
              << "Tags can annotate leafs, classes, fields, atoms, and more with a specific trait.  "
              << "See <a href=\"/help/tag.md\">HELP</a> for assistance."
              << "</div><p>\n";

            if(h.context().can_edit()){
                h << "<table align=\"right\" width=\"30%\"><tr><td>\n";
                new_tag_control(h, "/admin/tags/create");
                h << "</table>\n";
            }

            admin_table(h, cdb::all_tags(Sorted::YES));

        }

        void    p_admin_tags_create(WebContext& ctx)
        {
            if(!ctx.can_edit())
                throw HttpStatus::Unauthorized;
            
            ctx.decode_post();
            
            bool  edit_now      = ctx.edit_now();
            const RootDir* rt      = post::root_dir(ctx);
            if(!rt)
                throw HttpStatus::BadArgument;
            if(!rt->is_writable(DataRole::Config))
                throw HttpStatus::Forbidden;
                
            std::string     k   = post::key(ctx);
            if(k.empty())
                throw HttpStatus::BadArgument;
            
            
            bool    created = false;
            Tag     t = cdb::make_tag(k, rt, 0, &created);
            if(!t)
                throw HttpStatus::UnableToPerform;
            
            if(edit_now){
                Url url;
                url.path    = "/admin/tag";
                stream::Text    qu(url.query);
                qu << "id=" << t.id;
                if(rt)
                    qu << "&root_dir=" << rt->id;
                throw redirect::see_other(url);
            } else {
                ctx.return_to_sender();
            }
        }

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_tag(WebHtml& h)
        {
            Tag x   = tag(h);
            if(!x)
                throw HttpStatus::BadArgument;
            auto i = info(x);
            
            dev_title(h, x);
            auto ta = h.table();
            h.kvrow("ID") << x.id;
            h.kvrow("Name") << i.name;
            h.kvrow("Key") << i.key;
            h.kvrow("Brief") << i.brief;
            h.kvrow("Document") << dev(i.doc);
            h.kvrow("Leaf") << dev(leaf(x));
            h.kvrow("Image");
        }
        
        void    p_dev_tag_atoms(WebHtml& h)
        {
            Tag x   = tag(h);
            if(!x)
                throw HttpStatus::BadArgument;
            
            dev_title(h, x, "Atoms");
            dev_table(h, all_atoms(x));
        }

        void    p_dev_tag_classes(WebHtml& h)
        {
            Tag x   = tag(h);
            if(!x)
                throw HttpStatus::BadArgument;
            
            dev_title(h, x, "Classes");
            dev_table(h, classes_with_tag(x));
        }

        void    p_dev_tags(WebHtml& out)
        {
            out.title("All Tags");
            dev_table(out, all_tags());
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_tag_pages()
        {
            reg_webpage<p_api_tag>("/api/tag").argument("ID", "Tag ID");
            reg_webpage<p_api_tag_key>("/api/tag/key").argument("ID", "Tag ID");
            reg_webpage<p_api_tags>("/api/tags");

            reg_webpage<p_admin_tag>("/admin/tag").argument("id", "Tag ID");
            //  reg_webpage<p_admin_tags>("/admin/tags");  // registered in page.cpp
            reg_webpage<p_admin_tags_create>(hPost, "/admin/tags/create");

            reg_webgroup({
                reg_webpage<p_dev_tag>("/dev/tag")
                    .argument("id", "Tag id (number)")
                    .argument("key", "Tag key (string)")
                    .argument("tag", "Tag key/identifier").label("Info"),
                reg_webpage<p_dev_tag_atoms>("/dev/tag/atoms").argument("id", "Tag id (number)").label("Atoms"),
                reg_webpage<p_dev_tag_classes>("/dev/tag/classes").argument("id", "Tag id (number)").label("Classes")
            });
            reg_webpage<p_dev_tags>("/dev/tags");
            
            reg_webtemplate("/tag", wksp::shared("www/tag.ht"sv));
            reg_webtemplate("/tags", wksp::shared("www/tags.ht"sv));
            //reg_webpage<p_tags>("/tags");
        }
        
        YQ_INVOKE(reg_tag_pages();)
}
