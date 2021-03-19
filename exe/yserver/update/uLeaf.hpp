////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <db/AtomSys.hpp>
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

    ULeaf(Leaf);
    Image               explicit_icon() const;
};

ULeaf&          uget(Leaf);

