////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <kernel/atom/Field.hpp>
#include <kernel/file/Document.hpp>

namespace yq {
    struct UField {
        union {
            const uint64_t      id;
            const Field         field;
            const Document      doc;
        };
        bool                deleted = false;
        UField(Field a) : field(a) {}
    };

    //! Note, not thread-safe, call from ONE thread only!
    UField&  uget(Field);

    Field::SharedData       update(Field, cdb::cdb_options_t opts=0);
    void                    update_icon(Field);

}
