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
    struct MethodMeta;
}

namespace yq::mithril {
    struct WebContext;
    
    namespace arg {
        //! Finds method_info, tries key first, then by ID
        const MethodMeta* method_info(std::string_view arg_string);
        const MethodMeta* method_info(const WebContext&, bool *detected=nullptr);
        const MethodMeta* method_info(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const MethodMeta* method_info(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds method_info by ID only
        const MethodMeta* method_info_id(std::string_view arg_string);
        const MethodMeta* method_info_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const MethodMeta* method_info_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
