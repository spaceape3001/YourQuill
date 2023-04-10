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
    struct Root;
    struct Directory;
    
    namespace arg {

        //! Finds tag, tries key first, then by ID
        Directory directory(std::string_view arg_string, const Root* rt);
        Directory directory(const WebContext&, const Root* rt=nullptr, bool *detected=nullptr);
        Directory directory(const WebContext&, std::string_view arg_name, const Root* rt=nullptr, bool *detected=nullptr);
        Directory directory(const WebContext&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr, bool *detected=nullptr);

        //! Finds directory by ID only
        Directory directory_id(std::string_view arg_string);
        Directory directory_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Directory directory_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds directory by KEY only
        Directory directory_key(std::string_view arg_string, const Root* rt=nullptr);
        Directory directory_key(const WebContext&, std::string_view arg_name, const Root* rt=nullptr, bool *detected=nullptr);
        Directory directory_key(const WebContext&, std::initializer_list<std::string_view> arg_names, const Root* rt=nullptr, bool *detected=nullptr);
    }
}
