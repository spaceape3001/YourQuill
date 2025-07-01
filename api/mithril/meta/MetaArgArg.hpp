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
    struct ArgMeta;
}

namespace yq::mithril {
    struct WebContext;
    
    namespace arg {
        //! Finds arg_info, tries key first, then by ID
        const ArgMeta* arg_info(std::string_view arg_string);
        const ArgMeta* arg_info(const WebContext&, bool *detected=nullptr);
        const ArgMeta* arg_info(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const ArgMeta* arg_info(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds arg_info by ID only
        const ArgMeta* arg_info_id(std::string_view arg_string);
        const ArgMeta* arg_info_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const ArgMeta* arg_info_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
