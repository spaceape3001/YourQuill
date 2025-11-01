////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/html/Html.hpp>
#include <yq/net/Http.hpp>
#include <yq/mithril/web/WebPage.hpp>
#include <yq/mithril/web/WebVariable.hpp>

#include <initializer_list>
#include <string_view>
#include <vector>

namespace yq {
    struct ObjectMeta;
    class PropertyMeta;
    struct TypeMeta;
}

namespace yq::mithril {
    struct WebContext;
    class WebHtml;
    
    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const MetaBase*>);
        WebHtml&    operator<<(WebHtml&, Dev<const MetaBase*>);
    }

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const ObjectMeta*>);
        WebHtml&    operator<<(WebHtml&, Dev<const ObjectMeta*>);
        void    dev_table(WebHtml&, const std::vector<const ObjectMeta*>&);
        
    }

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const OperatorMeta*>);
        WebHtml&    operator<<(WebHtml&, Dev<const OperatorMeta*>);
        void    dev_table(WebHtml&, const std::vector<const OperatorMeta*>&);
    }

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const MethodMeta*>);
        WebHtml&    operator<<(WebHtml&, Dev<const MethodMeta*>);
        void    dev_table(WebHtml&, const std::vector<const MethodMeta*>&);
    }

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const PropertyMeta*>);
        WebHtml&    operator<<(WebHtml&, Dev<const PropertyMeta*>);
        void    dev_table(WebHtml&, const std::vector<const PropertyMeta*>&);
    }

    namespace html {
        WebHtml&    operator<<(WebHtml&, DevID<const TypeMeta*>);
        WebHtml&    operator<<(WebHtml&, Dev<const TypeMeta*>);
        void    dev_table(WebHtml&, const std::vector<const TypeMeta*>&);
        
    }

    using WebActionMethodPageMap    = Map<std::string_view, EnumMap<HttpOp, const WebPage*>, IgCase>;
    
    WebActionMethodPageMap  remapToActionMethods(const WebPageMap&);
    
    namespace html {
        void    dev_table(WebHtml&, const WebPageMap&, bool links=false);
        void    dev_table(WebHtml&, const WebVarMap&);
    }
}
