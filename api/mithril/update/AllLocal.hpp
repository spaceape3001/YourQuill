////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq::mithril::update {
    /*! \brief Used for distinguishing all vs locally defined resources
    */
    template <typename T>
    struct AllLocal {
        T   all;
        T   local;
        
        constexpr bool    operator==(const AllLocal&) const noexcept = default;
    };
}
