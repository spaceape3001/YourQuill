////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/html/Html.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Folder);
    WebHtml&    operator<<(WebHtml&, Dev<Folder>);
    WebHtml&    operator<<(WebHtml&, DevID<Folder>);
    WebHtml&    operator<<(WebHtml&, const Icon<Folder>&);

    void dev_table(WebHtml&, const FolderVector&);
    void dev_title(WebHtml&, Folder, std::string_view extra=std::string_view());
}


