////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/Html.hpp>

namespace yq {
    struct Fragment;
    namespace html {
        //WebHtml&    operator<<(WebHtml&, Fragment);
        WebHtml&    operator<<(WebHtml&, Dev<Fragment>);
        WebHtml&    operator<<(WebHtml&, DevID<Fragment>);
        void        dev_table(WebHtml&, const std::vector<Fragment>&);
    }
}


