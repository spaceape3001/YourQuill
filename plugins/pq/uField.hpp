////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"

#include <db/Field.hpp>
#include <db/Image.hpp>
#include <db/Tag.hpp>

struct UField {
    const String   key;
    union {
        const uint64_t  id;
        const Field     field;
    };
    //Image           icon;
    Class           cls;
    String          db;     //!< Attributes/atom table
    String          dbv;    //!< Value tracking table
    StringSet       types;
    ADSet<Class>    atoms;
    TagSet          tags;
    StringSet       aliases;
    bool            implied;
    
    UField(Field f);

    // Disable these
    UField(const UField&) = delete;
    UField(UField&&) = delete;
    UField& operator=(const UField&) = delete;
    UField& operator=(UField&&) = delete;
    ~UField() = delete;
};

UField&         uget(Field);
