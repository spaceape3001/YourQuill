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
    struct OperatorMeta;
}

namespace yq::mithril {
    struct WebContext;
    
    namespace arg {
        //! Finds operator_info, tries key first, then by ID
        const OperatorMeta* operator_info(std::string_view arg_string);
        const OperatorMeta* operator_info(const WebContext&, bool *detected=nullptr);
        const OperatorMeta* operator_info(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const OperatorMeta* operator_info(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds operator_info by ID only
        const OperatorMeta* operator_info_id(std::string_view arg_string);
        const OperatorMeta* operator_info_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const OperatorMeta* operator_info_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
