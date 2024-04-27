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

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

        void    p_admin_field(WebHtml&h)
        {
            Field f = field(h);
            if(!f)
                throw HttpStatus::BadArgument;
            
            h.title() << "Field " << key(f);
            h << "TODO Field " << label(f);
        }

        void    p_admin_fields(WebHtml& h)
        {
            h.title() << "Fields for [" << html_escape(wksp::name()) << "]";

            h << "<p><div class=\"explain\">"
              << "Fields declare the atom properties that are to be tracked in the cache database."
              << "See <a href=\"/help/field.md\">HELP</a> for assistance."
              << "</div><p>\n";

            if(h.context().can_edit()){
                h << "<table align=\"right\" width=\"30%\"><tr><td>\n";
                new_field_control(h, "/admin/fields/create");
                h << "</table>\n";
            }

            admin_table(h,  cdb::all_fields(Sorted::YES));
        }

        void    p_admin_fields_create(WebContext& ctx)
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
                
            Class           c   = post::class_(ctx);

            bool    created = false;
            Field     t = cdb::make_field(k, c, rt, 0, &created);
            if(!t)
                throw HttpStatus::UnableToPerform;
        
            if(edit_now){
                Url url;
                url.path    = "/admin/field";
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

        void    p_dev_field(WebHtml& h)
        {
            Field fld = field(h);
            if(!fld)
                throw HttpStatus::BadArgument;
            Field::Info i   = info(fld);
            dev_title(h, fld);
            auto ta = h.table();
            h.kvrow("ID") << fld.id;
            h.kvrow("Key") << i.key;
            h.kvrow("Class") << dev(i.class_);
            h.kvrow("Name") << i.name;
            h.kvrow("Plural") << i.plural;
            h.kvrow("Brief") << i.brief;
            h.kvrow("Category") << dev(i.category);
        }
        
        void    p_dev_field_tags(WebHtml& h)
        {
            Field fld = field(h);
            if(!fld)
                throw HttpStatus::BadArgument;
            dev_title(h, fld, "Tags");
            dev_table(h, tags(fld, Sorted::YES));
        }

        void    p_dev_fields(WebHtml& h)
        {
            h.title() << "All Fields";
            dev_table(h, all_fields(Sorted::YES));
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_field_pages()
        {
            reg_webpage<p_admin_field>("/admin/field").argument("id", "Field ID");
            // reg_webpage<p_admin_fields>("/admin/fields"); // registered in page.cpp
            reg_webpage<p_admin_fields_create>(hPost, "/admin/fields/create");

            reg_webgroup({
                reg_webpage<p_dev_field>("/dev/field").argument("id", "Field ID").label("Info"),
                reg_webpage<p_dev_field_tags>("/dev/field/tags").argument("id", "Field ID").label("Fields")
            });
            reg_webpage<p_dev_fields>("/dev/fields");
        }
}
