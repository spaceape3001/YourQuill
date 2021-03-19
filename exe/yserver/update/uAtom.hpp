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
    TagSet          tags;
    ClassSet        classes;
    Atom            src, tgt, rev;
    
    UAtom(Atom a);
    
    bool        update_classes(const StringSet&);
    bool        update_tags(const StringSet&);
};

UAtom&          uget(Atom);
