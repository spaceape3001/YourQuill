////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <yq/type/Enum.hpp>
#include <yq/type/Flag.hpp>

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
