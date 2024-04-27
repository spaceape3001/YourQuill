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

        void    dev_title(WebHtml& h, Style x, std::string_view extra=std::string_view())
        {
            auto t = h.title();
            auto i = cdb::nki(x);
            h << "Style \"" << i.name << "\" (" << x.id << " &mdash; " << i.key << ")";
            if(!extra.empty())
                h << ": " << extra;
        }
    
    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
        void    p_css(WebContext& ctx)
        {
            ctx.tx_content_type = ContentType::css;
            ctx.tx_content = gCss;
        }

    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

        void    p_admin_styles(WebHtml& h)
        {
            h.title() << "Styles for [" << html_escape(wksp::name()) << "]";

            h << "<p><div class=\"explain\">"
              << "Styles are about rendering output (including web) with specific traits.  "
              //<< "See <a href=\"/help/style.md\">HELP</a> for assistance."
              << "</div><p>\n";

            if(h.context().can_edit()){
                h << "<table align=\"right\" width=\"30%\"><tr><td>\n";
                new_style_control(h, "/admin/styles/create");
                h << "</table>\n";
            }

            admin_table(h, cdb::all_styles(Sorted::YES));

        }

        void    p_admin_style(WebHtml& h)
        {
            Style     t   = arg::style(h);
            if(!t)
                throw HttpStatus::BadArgument;
                
            h.title() << "Style (" << key(t) << ")";
            h << "TODO... style " << label(t);
        }

        void    p_admin_styles_create(WebContext& ctx)
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
            Style     t = cdb::make_style(k, rt, 0, &created);
            if(!t)
                throw HttpStatus::UnableToPerform;
            
            if(edit_now){
                Url url;
                url.path    = "/admin/style";
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

        void    p_dev_style(WebHtml& h)
        {
            Style   x = style(h);
            if(!x)
                throw HttpStatus::BadArgument;
            auto i = cdb::info(x);
            dev_title(h, x);
            auto ta = h.table();
            h.kvrow("ID") << x.id;
            h.kvrow("Name") << i.name;
            h.kvrow("Key") << i.key;
            h.kvrow("Brief") << i.brief;
            h.kvrow("Document") << dev(i.doc);
        }

        void    p_dev_styles(WebHtml& out)
        {
            out.title("All Styles");
            dev_table(out, all_styles());
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_style_pages()
        {
            reg_webpage<p_css>("/css");

            reg_webpage<p_admin_style>("/admin/style").argument("id", "Style ID");
            //  reg_webpage<p_admin_styles>("/admin/styles");  // registered in page.cpp
            reg_webpage<p_admin_styles_create>(hPost, "/admin/styles/create");

            reg_webpage<p_dev_style>("/dev/style").local().argument("id", "Style id (number)");
            reg_webpage<p_dev_styles>("/dev/styles");
        }
}
