////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <db/AtomSys.hpp>
#include <db/Leaf.hpp>
#include <util/Set.hpp>


    /*
        These are the data structures to persist (non-DB) during a run.  They won't be deleted until next run.
    */

struct uAtom {
    Atom            atom;
    
    uAtom(Atom a) : atom(a) {}
};

// These are our tracking data structures


struct uClass {
    Class           cls;
    Document        doc;
    Set<Class>      use, src, tgt, rev;
    Map<Class,int>  base, derives, sources, targets, reverses;
    Set<Field>      fields;
    
    uClass(Class c) : cls(c) {}
};

struct uField {
    Field           field;
    Class           def;
    QString         db;     //!< Attributes/atom table
    QString         vdb;    //!< Value tracking table
    
    uField(Field f) : field(f) {}
};


uAtom&          uget(Atom);
uClass&         uget(Class);
uField&         uget(Field);

void            init_leaf();
void            init_class();
void            init_class_graphs();



