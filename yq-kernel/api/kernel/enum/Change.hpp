////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/InfoBinder.hpp>
#include <basic/Enum.hpp>
#include <basic/Flag.hpp>

namespace yq {

    YQ_ENUM(Change, ,
        None,
        Added,      // fragment's been added
        Modified,   // fragment's been modified
        Removed     // fragment's been removed
    )
    
    using ChangeFlags   = Flag<Change>;
}

YQ_TYPE_DECLARE(yq::Change)
YQ_TYPE_DECLARE(yq::ChangeFlags)
