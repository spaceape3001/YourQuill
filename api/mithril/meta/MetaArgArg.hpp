////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <initializer_list>
#include <string_view>
#include <vector>

namespace yq {
    struct ArgInfo;
}

namespace yq::mithril {
    struct WebContext;
    
    namespace arg {
        //! Finds arg_info, tries key first, then by ID
        const ArgInfo* arg_info(std::string_view arg_string);
        const ArgInfo* arg_info(const WebContext&, bool *detected=nullptr);
        const ArgInfo* arg_info(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const ArgInfo* arg_info(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds arg_info by ID only
        const ArgInfo* arg_info_id(std::string_view arg_string);
        const ArgInfo* arg_info_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const ArgInfo* arg_info_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
