////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/http/Html.hpp>
#include <basic/Http.hpp>
#include <mithril/http/web/WebPage.hpp>
#include <mithril/http/web/WebVariable.hpp>

#include <initializer_list>
#include <string_view>
#include <vector>

namespace yq {
    struct WebContext;
    class WebHtml;
    struct ObjectInfo;
    class PropertyInfo;
    struct TypeInfo;
    

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const Meta*>);
        WebHtml&    operator<<(WebHtml&, Dev<const Meta*>);
    }

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const ObjectInfo*>);
        WebHtml&    operator<<(WebHtml&, Dev<const ObjectInfo*>);
        void    dev_table(WebHtml&, const std::vector<const ObjectInfo*>&);
        
    }

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const MethodInfo*>);
        WebHtml&    operator<<(WebHtml&, Dev<const MethodInfo*>);
        void    dev_table(WebHtml&, const std::vector<const MethodInfo*>&);
    }

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const PropertyInfo*>);
        WebHtml&    operator<<(WebHtml&, Dev<const PropertyInfo*>);
        void    dev_table(WebHtml&, const std::vector<const PropertyInfo*>&);
    }

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const TypeInfo*>);
        WebHtml&    operator<<(WebHtml&, Dev<const TypeInfo*>);
        void    dev_table(WebHtml&, const std::vector<const TypeInfo*>&);
        
    }

    using WebActionMethodPageMap    = Map<std::string_view, EnumMap<HttpOp, const WebPage*>, IgCase>;
    
    WebActionMethodPageMap  remapToActionMethods(const WebPageMap&);
    
    namespace html {
        void    dev_table(WebHtml&, const WebPageMap&, bool links=false);
        void    dev_table(WebHtml&, const WebVarMap&);
    }
}
