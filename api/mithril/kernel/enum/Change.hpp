////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/InfoBinder.hpp>
#include <basic/Enum.hpp>
#include <basic/Flag.hpp>

namespace yq {
    namespace mithril {
        YQ_ENUM(Change, ,
            None,
            Added,      // fragment's been added
            Modified,   // fragment's been modified
            Removed     // fragment's been removed
        )
        
        using ChangeFlags   = Flag<Change>;
    }
}

YQ_TYPE_DECLARE(yq::mithril::Change)
YQ_TYPE_DECLARE(yq::mithril::ChangeFlags)
