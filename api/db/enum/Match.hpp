////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/meta/Binder.hpp>
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
}

YQ_TYPE_DECLARE(yq::Match)
