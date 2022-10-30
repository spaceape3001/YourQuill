////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    p_dev_webdirs(WebHtml&h)
    {
        h.title("All WebDirs");
        dev_table(h, web::directory_map());
    }

    void p_dev_webexts(WebHtml& h)
    {
        h.title("All WebExts");
        dev_table(h, web::extension_map());
    }
    
    void p_dev_webglobs(WebHtml& h)
    {
        h.title("All WebGlobs");
        dev_table(h, web::glob_map());
    }

    void p_dev_webpages(WebHtml& h)
    {
        h.title("All WebPages");
        
        h << "<div class=\"explain\">"
          << "List of all registered (specific) web-pages.  Directories & GLOBs not included."
          << "</div>\n";
        
        dev_table(h, web::page_map(), true);
    }

    void p_dev_webvars(WebHtml& h)
    {
        h.title("All WebVariables");
        dev_table(h, web::variable_map());
    }

    void    reg_dev_web()
    {
        reg_webpage<p_dev_webdirs>("/dev/web/directories");
        reg_webpage<p_dev_webexts>("/dev/web/extensions"); 
        reg_webpage<p_dev_webglobs>("/dev/web/globs");
        reg_webpage<p_dev_webpages>("/dev/web/pages");
        reg_webpage<p_dev_webvars>("/dev/web/variables");
    }
}
