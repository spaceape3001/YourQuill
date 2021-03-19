////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <db/AtomSys.hpp>
#include <db/Tag.hpp>

struct UAtom {
    const QString   key;
    union {
        const uint64_t  id;
        const Atom      atom;
    };
    Image           icon;       // explicit icon
    TagSet          tags;
    ClassSet        classes;
    Atom            src, tgt, rev;
    
    UAtom(Atom a);
    
    Image       update_icon(Image);
    bool        update_classes(const StringSet&);
    bool        update_tags(const StringSet&);


        // DISABLE THESE....
    ~UAtom() = delete;
    UAtom(const UAtom&) = delete;
    UAtom(UAtom&&) = delete;
    UAtom& operator=(const UAtom&) = delete;
    UAtom& operator=(UAtom&&) = delete;
};

UAtom&          uget(Atom);
