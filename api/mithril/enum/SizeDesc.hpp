////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/MetaBinder.hpp>
#include <yq/core/Enum.hpp>

namespace yq::mithril {

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
}

YQ_TYPE_DECLARE(yq::mithril::SizeDesc)
