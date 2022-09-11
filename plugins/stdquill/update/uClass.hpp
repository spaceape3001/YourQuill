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
        
        URefSet<Class>          base;
        URefSet<Class>          derived;
        URefSet<Class>          source;
        URefSet<Class>          target;
        URefSet<Class>          reverse;
        URefSet<Field>          fields;
        UDefResolved<Image>     icon;
        Class::SharedData       data;
        UDefResolved<Category>  category;
        
        bool                    edge    = false;
        bool                    deleted = false;
        
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

    Class::SharedData           update(Class, cdb_options_t opts=0);
    void                        update_icon(Class);
}
