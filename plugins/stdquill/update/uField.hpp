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
    struct UField : trait::not_copyable, trait::not_moveable {
        const std::string       k;
        union {
            const uint64_t      id;
            const Field         field;
            const Document      doc;
        };
        bool                deleted = false;
        explicit UField(Field a);

        //! Creates the database-lookups only
        static void             s3_create(Document);
    };

    //! Note, not thread-safe, call from ONE thread only!
    UField&  uget(Field);

    Field::SharedData       update_this(Field, cdb_options_t opts=0);

}
