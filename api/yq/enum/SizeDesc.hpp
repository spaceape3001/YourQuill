////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <yq/type/Enum.hpp>

#include <initializer_list>
#include <string_view>

namespace yq {

    /*! 
        \enum SizeDesc
        \brief Generic size descriptor (for end users)
    */
    YQ_ENUM(SizeDesc, ,
        Original,
        Large,
        Medium,
        Small
    )
    
    struct WebContext;
    
    namespace arg {
        SizeDesc    size_desc(std::string_view);
        SizeDesc    size_desc(const WebContext&, std::string_view arg_name, bool *detected=nullptr);
        SizeDesc    size_desc(const WebContext&, std::initializer_list<std::string_view> arg_names, bool *detected=nullptr);
    }

}

YQ_TYPE_DECLARE(yq::SizeDesc)
