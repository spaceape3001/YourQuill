////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <db/AtomSys.hpp>
#include <db/Image.hpp>
#include <db/Leaf.hpp>
#include <util/Set.hpp>


static constexpr const std::initializer_list<const char*>    kImages = { "svg", "png", "gif", "jpg", "bmp", "tif", "tiff" };

    /*
        These are the data structures to persist (non-DB) during a run.  They won't be deleted until next run.
    */

//struct uAtom {
    //Atom            atom;
    
    //uAtom(Atom a) : atom(a) {}
//};

// These are our tracking data structures

struct UClass;

template <typename T>
struct ADSet {
    Set<T>      def;
    Set<T>      all;
};

using ClassSet      = Set<Class>;
using FieldSet      = Set<Field>;
using TagSet        = Set<Tag>;

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
    const Folder        folder;
    Image               icon;
    ClassData::Shared   data;
    TagSet              tags;
    
    ADSet<Class>        use, der, src, tgt, rev;
    ADSet<Field>        fields;
    ClassSet            sources, targets, reverses; // "defined" list, either direct or inherited
    
    UClass(Class c);
};

struct ULeaf {
    const QString       key;
    union {
        const uint64_t      id;
        const Leaf          leaf;
        const Document      doc;
    };
    Image               icon;
    ClassSet            classes;
    TagSet              tags;
    
    ULeaf(Leaf);
};


//uAtom&          uget(Atom);
UClass&         uget(Class);
UField&         uget(Field);
//ULeaf&          uget(Leaf);

void            init_leaf();
void            init_class();
void            init_class_graphs();

Image           icon_for(Folder, const QString&);


