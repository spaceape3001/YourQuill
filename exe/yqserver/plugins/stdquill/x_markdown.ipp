////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_extension_markdown( WebHtml& out )
    {
        WebContext&ctx  = out.context();
        std::string     bytes   = file_string(ctx.resolved_file);
        if(bytes.empty())
            throw hNotFound;

        std::string_view    t   = web_title(bytes);
        std::string_view    d;
        
        if(!t.empty()){
            const char* z0  = t.data()+t.size();
            const char* z1  = bytes.data() + bytes.size();
            d               = std::string_view(z0, z1);
            ctx.var_title   = trimmed(t);
        } else {
            d               = bytes;
            ctx.var_title   = ctx.resolved_file.filename().string();
        }
        
        out << markdown(d);

        //stream::Text    render(ctx.var_body);
        //Markdown::Config    config;
        //Markdown            parser(render, config);
        //parser.process(d);
    }

    void    reg_x_markdown()
    {
        reg_webpage<page_extension_markdown>("*.md");
    }
}
