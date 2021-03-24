////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"

#include <db/Class.hpp>
#include <db/Image.hpp>
#include <db/Tag.hpp>

using CPair         = std::pair<Class,Class>;

struct UClass {
    const QString       key;
    union {
        const uint64_t      id;
        const Class         cls;
        const Document      doc;
    };
    const Folder        folder;     // config/sub
    Image               icon;
    ClassData::Shared   data;
    Set<Tag>            tags;
    
    ADSet<Class>        use, der, src, tgt, rev;
    ADSet<Field>        fields;
    Set<Class>          sources, targets, reverses; // "defined" list, either direct or inherited
    Set<CPair>          cxn;                 // connections through this edge
    Map<Class,Set<Class>> in, out;                    // in/outbound class types
    bool                edge;
    QString             db; //!< For edges
    
    UClass(Class c);
    
    //!  Determines the icon that's best suited for this class.
    Image               calc_icon() const;
    
    //!  Determines the icon that's *explicitly* tied to this class
    Image               explicit_icon() const;


    // DISABLE THESE
    ~UClass() = delete;
    UClass(const UClass&) = delete;
    UClass(UClass&&) = delete;
    UClass& operator=(const UClass&) = delete;
    UClass& operator=(UClass&&) = delete;
};

UClass&         uget(Class);

void            init_class();
void            init_class_graphs();
Image           calc_icon_for(Class);
void            update_class_icon(Fragment);
