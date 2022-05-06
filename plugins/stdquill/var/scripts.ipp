////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/net/Http.hpp>
#include <yq/io/file_utils.hpp>

namespace {

    void    add_script(WebHtml& str, std::string_view name)
    {
        switch(mimeTypeForExt(file_extension(name))){
        case ContentType::javascript:
            str << "\t\t<script srce=\"/js/" << name << "\">\n";
            break;
        default:
            //  do nothing if unclear...
            break;
        }
    }
    
    void    var_scripts(WebHtml& h)
    {
        WebContext& ctx = h.context();
        for(std::string_view s : ctx.page->scripts())
            add_script(h, s);
        for(const std::string& s : ctx.var_scripts)
            add_script(h, s);
    }

    YQ_INVOKE( reg_webvar<var_scripts>("scripts"); )
}

