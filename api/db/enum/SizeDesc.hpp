////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/meta/Binder.hpp>
#include <util/type/Enum.hpp>

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
