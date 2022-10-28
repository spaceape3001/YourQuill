////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_css(WebContext& ctx)
    {
        ctx.tx_content_type = ContentType::css;
        ctx.tx_content = gCss;
    }

    void    reg_css()
    {
        reg_webpage<page_css>("/css");
    }
}
