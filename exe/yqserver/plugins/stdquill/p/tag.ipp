////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

        void    dev_title(WebHtml& h, Tag x, std::string_view extra=std::string_view())
        {
            auto t = h.title();
            auto i = cdb::nki(x);
            h << "Tag \"" << i.name << "\" (" << x.id << " &mdash; " << i.key << ")";
            if(!extra.empty())
                h << ": " << extra;
        }
    
    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

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

        void    p_admin_tag(WebHtml& h)
        {
            Tag     t   = arg::tag(h);
            if(!t)
                throw HttpStatus::BadArgument;
                
            h.title() << "Tag (" << key(t) << ")";
            h << "TODO... tag " << label(t);
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
        }
}
