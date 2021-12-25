////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/MetaValue.hpp>
#include <util/type/Enum.hpp>

namespace yq {

    /*! \brief Page matching

    */
    YQ_ENUM(Match,,
        //!  exact match
        Exact,
        //! match name
        Name,
        //! match extension
        Ext,
        //! any directory (w/o ".")
        AnyDir,
        //! any fragment (w/o ".")
        NoExt
    );
    MV_DECLARE(Match)

}
