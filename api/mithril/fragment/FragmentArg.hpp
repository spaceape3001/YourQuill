////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq::mithril {
    struct WebContext;
    class WebHtml;
    struct RootDir;
    struct Fragment;
    
    namespace arg {

        //! Finds fragment, tries key first, then by ID
        Fragment fragment(std::string_view arg_string, const RootDir* rt);
        Fragment fragment(const WebContext&, const RootDir* rt=nullptr, bool *detected=nullptr);
        Fragment fragment(const WebContext&, std::string_view arg_name, const RootDir* rt=nullptr, bool *detected=nullptr);
        Fragment fragment(const WebContext&, std::initializer_list<std::string_view> arg_names, const RootDir* rt=nullptr, bool *detected=nullptr);

        //! Finds fragment by ID only
        Fragment fragment_id(std::string_view arg_string);
        Fragment fragment_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Fragment fragment_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds fragment by KEY only
        Fragment fragment_key(std::string_view arg_string, const RootDir* rt);
        Fragment fragment_key(const WebContext&, std::string_view arg_name, const RootDir* rt=nullptr, bool *detected=nullptr);
        Fragment fragment_key(const WebContext&, std::initializer_list<std::string_view> arg_names, const RootDir* rt=nullptr, bool *detected=nullptr);
    }
}
