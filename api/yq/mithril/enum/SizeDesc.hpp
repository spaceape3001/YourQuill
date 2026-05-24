////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/MetaBinder.hpp>

namespace yq::mithril {

    /*! 
        \enum SizeDesc
        \brief Generic size descriptor (for end users)
    */
    enum class SizeDesc {
        Original,
        Large,
        Medium,
        Small
    };
}

YQ_TYPE_DECLARE(yq::mithril::SizeDesc)
