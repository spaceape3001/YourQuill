////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/html/Html.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Directory);
    WebHtml&    operator<<(WebHtml&, Dev<Directory>);
    WebHtml&    operator<<(WebHtml&, DevID<Directory>);
    
    void dev_table(WebHtml&, const DirectoryVector& dirs);
    void dev_title(WebHtml&, Directory, std::string_view extra=std::string_view());
}

