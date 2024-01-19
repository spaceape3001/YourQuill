////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq::mithril {
    /*! \brief Used for distinguishing defined vs resolved resources
    */
    template <typename T>
    struct DefRes {
        T   defined;
        T   resolved;
        
        constexpr bool    operator==(const DefRes&) const noexcept = default;
    };
}
