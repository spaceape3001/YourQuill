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

            reg_webpage<p_dev_style>("/dev/style").local().argument("id", "Style id (number)");
            reg_webpage<p_dev_styles>("/dev/styles");
        }
}
