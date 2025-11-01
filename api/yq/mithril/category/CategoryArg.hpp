////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>

namespace yq::mithril {
    struct Category;
    struct WebContext;
    
    namespace arg {
        //! Finds category, tries key first, then by ID
        Category category(std::string_view arg_string);
        Category category(const WebContext&, bool *detected=nullptr);
        Category category(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Category category(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds category by ID only
        Category category_id(std::string_view arg_string);
        Category category_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Category category_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds category by KEY only
        Category category_key(std::string_view arg_string);
        Category category_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        Category category_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}

