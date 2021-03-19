////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"

#include <db/AtomSys.hpp>
#include <db/Image.hpp>
#include <db/Tag.hpp>

struct UField {
    const QString   key;
    union {
        const uint64_t  id;
        const Field     field;
    };
    //Image           icon;
    Class           cls;
    QString         db;     //!< Attributes/atom table
    QString         dbv;    //!< Value tracking table
    StringSet       types;
    ADSet<Class>    atoms;
    TagSet          tags;
    StringSet       aliases;
    bool            implied;
    
    UField(Field f);
};

UField&         uget(Field);
