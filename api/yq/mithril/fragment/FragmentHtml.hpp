////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/html/Html.hpp>

namespace yq::mithril::html {
    //WebHtml&    operator<<(WebHtml&, Fragment);
    WebHtml&    operator<<(WebHtml&, Dev<Fragment>);
    WebHtml&    operator<<(WebHtml&, DevID<Fragment>);

    void dev_table(WebHtml&, const FragmentVector&);
    void dev_title(WebHtml&, Fragment, std::string_view extra=std::string_view());
}

