////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <basic/Enum.hpp>

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

}

YQ_TYPE_DECLARE(yq::SizeDesc)
