////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/Binder.hpp>
#include <yq/type/Enum.hpp>
#include <yq/type/Flag.hpp>

namespace yq {

    YQ_ENUM(Change, ,
        None,
        Startup,    // on startup pass
        Added,      // fragment's been added
        Modified,   // fragment's been modified
        Removed     // fragment's been removed
    )
    
    using ChangeFlags   = Flag<Change>;
}

YQ_TYPE_DECLARE(yq::Change)
YQ_TYPE_DECLARE(yq::ChangeFlags)
