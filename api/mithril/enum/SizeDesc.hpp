////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/InfoBinder.hpp>
#include <basic/Enum.hpp>

namespace yq {
    namespace mithril {

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
}

YQ_TYPE_DECLARE(yq::mithril::SizeDesc)