////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    json    p_api_wksp(WebContext&ctx)
    {
        json    ret{
            { "author", wksp::author() },
            { "abbreviation", wksp::abbreviation() },
            { "bkcolor", gBkColor },
            { "copyright", wksp::copyright().text },
            { "c_stance", wksp::copyright().stance.key() },
            { "c_from", wksp::copyright().from },
            { "c_to", wksp::copyright().to },
            { "color", gTextColor },
            { "name", wksp::name() }
        };
        if(ctx.is_local()){
            ret["quill"] = wksp::quill_file().string();
            ret["cache"] = wksp::cache().string();
        }
        return ret;
    }

    json    p_api_wksp_quill(WebContext&ctx)
    {
        json    ret;
        if(ctx.is_local()){
            ret["quill"] = wksp::quill_file().string();
        }
        return ret;
    }

    void    reg_api_wksp()
    {
        reg_webpage<p_api_wksp>("/api/wksp"sv); 
        reg_webpage<p_api_wksp_quill>("/api/wksp/quill"sv); 
    }
}
