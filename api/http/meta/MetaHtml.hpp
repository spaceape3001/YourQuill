////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <http/Html.hpp>
#include <basic/Http.hpp>
#include <http/web/WebPage.hpp>
#include <http/web/WebVariable.hpp>

#include <initializer_list>
#include <string_view>
#include <vector>

namespace yq {
    struct WebContext;
    class WebHtml;
    struct ObjectInfo;
    class PropertyInfo;
    struct TypeInfo;
    
    namespace arg {
        //! Finds object_info, tries key first, then by ID
        const ObjectInfo* object_info(std::string_view arg_string);
        const ObjectInfo* object_info(const WebContext&, bool *detected=nullptr);
        const ObjectInfo* object_info(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const ObjectInfo* object_info(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds object_info by ID only
        const ObjectInfo* object_info_id(std::string_view arg_string);
        const ObjectInfo* object_info_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const ObjectInfo* object_info_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds object_info by key only
        const ObjectInfo* object_info_key(std::string_view arg_string);
        const ObjectInfo* object_info_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const ObjectInfo* object_info_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }

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

    namespace arg {
        //! Finds type_info, tries key first, then by ID
        const TypeInfo* type_info(std::string_view arg_string);
        const TypeInfo* type_info(const WebContext&, bool *detected=nullptr);
        const TypeInfo* type_info(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const TypeInfo* type_info(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds type_info by ID only
        const TypeInfo* type_info_id(std::string_view arg_string);
        const TypeInfo* type_info_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const TypeInfo* type_info_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds type_info by key only
        const TypeInfo* type_info_key(std::string_view arg_string);
        const TypeInfo* type_info_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const TypeInfo* type_info_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
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
