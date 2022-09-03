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

    Class::SharedData           update(Class, cdb_options_t opts=0);
    void                        update_icon(Class);
}
