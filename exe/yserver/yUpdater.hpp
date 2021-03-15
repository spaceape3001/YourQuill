////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <db/AtomSys.hpp>
#include <db/Image.hpp>
#include <db/Leaf.hpp>
#include <db/Tag.hpp>
#include <util/Set.hpp>


static constexpr const std::initializer_list<const char*>    kImages = { "svg", "png", "gif", "jpg", "bmp", "tif", "tiff" };

    /*
        These are the data structures to persist (non-DB) during a run.  They won't be deleted until next run.
        
        (They're also a crutch until the dtabase gets super-good)
    */
using ClassSet      = Set<Class>;
using FieldSet      = Set<Field>;
using TagSet        = Set<Tag>;
using CPair         = std::pair<Class,Class>;

template <typename T>
struct ADSet {
    Set<T>      def;
    Set<T>      all;
};
    
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

struct UClass;


struct UField {
    const QString   key;
    union {
        const uint64_t  id;
        const Field     field;
    };
    Image           icon;
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
    TagSet              tags;
    
    ADSet<Class>        use, der, src, tgt, rev;
    ADSet<Field>        fields;
    ClassSet            sources, targets, reverses; // "defined" list, either direct or inherited
    Set<CPair>          cxn;                 // connections through this edge
    Map<Class,ClassSet> in, out;                    // in/outbound class types
    bool                edge;
    QString             db; //!< For edges
    
    UClass(Class c);
};

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
UClass&         uget(Class);
UField&         uget(Field);
ULeaf&          uget(Leaf);

void            init_leaf();
void            init_class();
void            init_class_graphs();

Image           icon_for(Folder, const QString&);


