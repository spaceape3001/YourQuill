////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "update/uFwd.hpp"

#include <db/AtomSys.hpp>
#include <db/Image.hpp>
#include <db/Leaf.hpp>
#include <db/Tag.hpp>


//static constexpr const std::initializer_list<const char*>    kImages = { "svg", "png", "gif", "jpg", "bmp", "tif", "tiff" };

    /*
        These are the data structures to persist (non-DB) during a run.  They won't be deleted until next run.
        
        (They're also a crutch until the dtabase gets super-good)
    */
using ClassSet      = Set<Class>;
using FieldSet      = Set<Field>;
using TagSet        = Set<Tag>;


struct UAtom {
    const QString   key;
    union {
        const uint64_t  id;
        const Atom      atom;
    };
    TagSet          tags;
    ClassSet        classes;
    Atom            src, tgt, rev;
    
    UAtom(Atom a);
    
    bool        update_classes(const StringSet&);
    bool        update_tags(const StringSet&);
};

// These are our tracking data structures


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
    Image                   icon;
    TagSet                  tags;
    ULeaf(Leaf);
};


UAtom&          uget(Atom);
ULeaf&          uget(Leaf);

void            init_leaf();

Image           calc_icon_for(Folder, const QString&);
Image           calc_icon_for(Field);
Image           calc_icon_for(const Root*);

void            update_field_icon(Fragment);



