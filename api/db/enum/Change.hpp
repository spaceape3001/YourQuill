////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/MetaValue.hpp>
#include <util/type/Enum.hpp>

namespace yq {

    YQ_ENUM(Change, ,
        None,
        Startup,    // on startup pass
        Added,      // fragment's been added
        Modified,   // fragment's been modified
        Removed     // fragment's been removed
    )
    MV_DECLARE(Change)
}
