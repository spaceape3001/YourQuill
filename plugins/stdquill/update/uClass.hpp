////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <kernel/atom/Class.hpp>
#include <kernel/atom/Field.hpp>
#include <kernel/file/Document.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/org/Category.hpp>
#include <kernel/org/Tag.hpp>

namespace yq {
    struct UClass : trait::not_copyable, trait::not_moveable  {
        const std::string       k;
        union {
            const uint64_t      id;
            const Class         cls;
            const Document      doc;
        };
        
        URefSet<Class>          base;       // In cache
        URefSet<Class>          derived;    // In cache
        URefSet<Class>          source;     // Don't need these
        URefSet<Class>          target;     // Don't need these
        URefSet<Class>          reverse;    // Don't need these
        URefSet<Field>          fields;     // In cache
        UDefResolved<Image>     icon;           //  in cache...
        Class::SharedData       data;           //  nope, jettison
        UDefResolved<Category>  category;       //  in cache
        
        bool                    edge    = false;    // in cache
        bool                    deleted = false;    // not an issue
        
        explicit UClass(Class a);
        
        bool                    reload(cdb_options_t opts=0);
        
        void                    u_header();
        
        //! Creates the database-lookups only
        static void             s3_create(Document);
        
        //! Initializes the class
        static void             s3_initialize(Document);
    };

    //! Note, not thread-safe, call from ONE thread only!
    UClass&  uget(Class);

    Class::SharedData           update_this(Class, cdb_options_t opts=0);
}
