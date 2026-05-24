////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/MetaBinder.hpp>

namespace yq::mithril {

    /*! \brief Access amount 
    */
    enum class Access : uint8_t {
        Default,
        NoAccess,
        ReadOnly,
        ReadWrite,
            //  if set, prefer to write in this location *first* on specified
            //  information
        WriteFirst,
        
        First=WriteFirst,
        Write=ReadWrite,
        Read=ReadOnly,
        Deny=NoAccess,
        None=NoAccess
    };
}

YQ_TYPE_DECLARE(yq::mithril::Access)
