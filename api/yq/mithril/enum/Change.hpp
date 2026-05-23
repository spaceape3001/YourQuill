////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/MetaBinder.hpp>
#include <yq/core/Flags.hpp>

namespace yq::mithril {
    enum class Change : uint8_t {
        None,
        Added,      // fragment's been added
        Modified,   // fragment's been modified
        Removed     // fragment's been removed
    };
    
    using ChangeFlags   = Flags<Change,uint8_t>;
}

YQ_TYPE_DECLARE(yq::mithril::Change)
YQ_TYPE_DECLARE(yq::mithril::ChangeFlags)
