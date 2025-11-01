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
    struct PropertyMeta;
}

namespace yq::mithril {
    struct WebContext;
    
    namespace arg {
        //! Finds property_info, tries key first, then by ID
        const PropertyMeta* property_info(std::string_view arg_string);
        const PropertyMeta* property_info(const WebContext&, bool *detected=nullptr);
        const PropertyMeta* property_info(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const PropertyMeta* property_info(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);

        //! Finds property_info by ID only
        const PropertyMeta* property_info_id(std::string_view arg_string);
        const PropertyMeta* property_info_id(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        const PropertyMeta* property_info_id(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }
}
