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
        
        void    p_graphviz(WebHtml& h)
        {
            Graphviz    gviz = graphviz(h);
            if(!gviz)
                throw HttpStatus::BadArgument;
            h.title() << cdb::name(gviz);
            h << gviz;
        }

        void    p_graphviz_svg(WebContext& ctx)
        {
            Graphviz    gviz = graphviz(ctx);
            if(!gviz)
                throw HttpStatus::BadArgument;
            ctx.tx_content_type = ContentType::svg;
            ctx.tx_content      = std::make_shared<ByteArray>(cdb::svg_bytes(gviz));
        }

    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void p_dev_graphviz(WebHtml&h)
        {
            Graphviz    gv  = graphviz(h);
            if(!gv)
                throw HttpStatus::BadArgument;
            
            dev_title(h, gv);
            auto t = h.table();
            h.kvrow("Name") << cdb::name(gv);
            h.kvrow("Dot Size") << cdb::dot_size(gv);
            h.kvrow("Html Size") << cdb::html_size(gv);
            h.kvrow("SVG Size") << cdb::svg_size(gv);
        }
        
        void    p_dev_graphviz_html(WebHtml&h)
        {
            Graphviz    gv  = graphviz(h);
            if(!gv)
                throw HttpStatus::BadArgument;

            dev_title(h, gv, "HTML");
            std::string txt = cdb::html_text(gv);
            h << "<pre>\n";
            html_escape_write(h, txt);
            h << "</pre>\n";
        }

        void    p_dev_graphviz_dot(WebHtml&h)
        {
            Graphviz    gv  = graphviz(h);
            if(!gv)
                throw HttpStatus::BadArgument;
            dev_title(h, gv, "DOT");
            std::string txt = cdb::dot_text(gv);
            h << "<pre>\n";
            html_escape_write(h, txt);
            h << "</pre>\n";
        }

        void    p_dev_graphviz_svg(WebHtml&h)
        {
            Graphviz    gv  = graphviz(h);
            if(!gv)
                throw HttpStatus::BadArgument;
            dev_title(h, gv, "SVG");
            h << gv;
        }
        
        void p_dev_graphvizs(WebHtml& h)
        {
            h.title("All Graphvizs");
            dev_table(h, all_graphvizs());
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_graphviz_pages()
        {
            reg_webpage<p_graphviz>("/graphviz").argument("id", "ID for the graphviz").description("Shows a graphviz graph");
            reg_webpage<p_graphviz_svg>("/graphviz/svg").argument("id", "ID for the graphviz").description("Graphviz SVG");

            reg_webgroup({
                reg_webpage<p_dev_graphviz>("/dev/graphviz").description("Info for a graphviz").argument("id", "Graphviz ID").label("Info"),
                reg_webpage<p_dev_graphviz_dot>("/dev/graphviz/dot").description("DOT for a graphviz").argument("id", "Graphviz ID").label("DOT"),
                reg_webpage<p_dev_graphviz_html>("/dev/graphviz/html").description("HTML for a graphviz").argument("id", "Graphviz ID").label("HTML"),
                reg_webpage<p_dev_graphviz_svg>("/dev/graphviz/svg").description("SVG for a graphviz").argument("id", "Graphviz ID").label("SVG")
            });
            reg_webpage<p_dev_graphvizs>("/dev/graphvizs");
        }
}
