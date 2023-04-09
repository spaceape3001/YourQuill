////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace update {
        template <typename T>
        struct DefRes {
            T   defined;
            T   resolved;
            
            bool    operator==(const DefRes&) const noexcept = default;
        };
    }
}
