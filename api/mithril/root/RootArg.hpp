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
    struct RootDir;
    
    namespace arg {
        //! Finds root_dir, tries key first, then by ID
        const RootDir* root_dir(std::string_view arg_string);
        const RootDir* root_dir(const WebContext&, bool *detected=nullptr);
        const RootDir* root_dir(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const RootDir* root_dir(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds root_dir by ID only
        const RootDir* root_id(std::string_view arg_string);
        const RootDir* root_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const RootDir* root_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
        
        //! Finds root_dir by key only
        const RootDir* root_key(std::string_view arg_string);
        const RootDir* root_key(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const RootDir* root_key(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
