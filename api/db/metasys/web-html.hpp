////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/net/Http.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebVariable.hpp>
#include <vector>

namespace yq {
    using WebActionMethodPageMap    = Map<std::string_view, EnumMap<HttpOp, const WebPage*>, IgCase>;
    
    WebActionMethodPageMap  remapToActionMethods(const WebPageMap&);
    
    namespace html {
        void    dev_table(WebHtml&, const WebPageMap&, bool links=false);
        void    dev_table(WebHtml&, const WebVarMap&);
    }
}
