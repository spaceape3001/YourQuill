////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <db/Atom.hpp>
#include <db/Class.hpp>
#include <db/FileSys.hpp>
#include <db/Leaf.hpp>


struct ULeaf {
    const QString           key;
    union {
        const uint64_t      id;
        const Leaf          leaf;
        const Document      doc;
    };
    const Folder            folder;     // config/sub
    const Atom              atom;
    ClassSet                classes;
    Leaf::Merge::Shared     data;       // this is attributes-only
    TagSet                  tags;
    Image                   icon;       // explicit icon

    static void         init_link();
    static void         init_read();
    static void         on_image_change(Fragment);
    static void         on_leaf_change(Fragment);

    ULeaf(Leaf);
    
    Image               explicit_icon() const;
    void                do_icon();
    void                do_link();
    void                do_read();
    
    void                push_icon();
    
    // DISABLE THESE
    ~ULeaf() = delete;
    ULeaf(const ULeaf&) = delete;
    ULeaf(ULeaf&&) = delete;
    ULeaf& operator=(const ULeaf&) = delete;
    ULeaf& operator=(ULeaf&&) = delete;
};

ULeaf&          uget(Leaf);

