////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq {
    struct WebContext;
    class WebHtml;
    struct Root;
    struct Fragment;
    
    namespace arg {

        //! Finds fragment, tries key first, then by ID
        Fragment fragment(std::string_view arg_string, const Root* rt=nullptr);
        Fragment fragment(const WebContext&, const Root* rt=nullptr);
        Fragment fragment(const WebHtml&, const Root* rt=nullptr);
        Fragment fragment(const WebContext&, std::string_view arg_name, const Root* rt=nullptr);
        Fragment fragment(const WebHtml&, std::string_view arg_name, const Root* rt=nullptr);
        Fragment fragment(const WebContext&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);
        Fragment fragment(const WebHtml&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);

        //! Finds fragment by ID only
        Fragment fragment_id(std::string_view arg_string);
        Fragment fragment_id(const WebContext&, std::string_view arg_name);
        Fragment fragment_id(const WebHtml&, std::string_view arg_name);
        Fragment fragment_id(const WebContext&, std::initializer_list<std::string_view> arg_names);
        Fragment fragment_id(const WebHtml&, std::initializer_list<std::string_view> arg_names);
        
        //! Finds fragment by KEY only
        Fragment fragment_key(std::string_view arg_string, const Root* rt=nullptr);
        Fragment fragment_key(const WebContext&, std::string_view arg_name, const Root* rt=nullptr);
        Fragment fragment_key(const WebHtml&, std::string_view arg_name, const Root* rt=nullptr);
        Fragment fragment_key(const WebContext&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);
        Fragment fragment_key(const WebHtml&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr);
    }
}
