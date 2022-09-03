////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <kernel/atom/Class.hpp>
#include <kernel/file/Document.hpp>

namespace yq {
    namespace update {
        struct UClass {
            union {
                const uint64_t      id;
                const Class         cls;
                const Document      doc;
            };
            bool                deleted = false;
            UClass(Class a) : cls(a) {}
        };

        //! Note, not thread-safe, call from ONE thread only!
        UClass&  uget(Class);
    }
}
