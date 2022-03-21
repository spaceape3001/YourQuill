////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <yq/Atom.hpp>
#include <yq/Attribute.hpp>
#include <yq/Property.hpp>
#include <yq/Tag.hpp>

namespace cdb {
    struct KVReport;
};

//struct UAtom {
    //const QString   key;
    //Image           icon;       // explicit icon
    //ClassSet        classes;
    //Atom            src, tgt, rev;
    
    //UAtom(Atom a);
    
    //Image       update_icon(Image);
    //bool        update_classes(const StringSet&);
    //bool        update_tags(const StringSet&);

    ///*! \brief Updates the attributes with what's changed in the specified document
        //\param  attributes  Attributes for *THIS* atom
        //\param  common      Common attributes within the file (ie, for a roster, etc)
    //*/
    //bool        update_properties(Document doc, const cdb::KVReport& attributes, const cdb::KVReport& common=cdb::KVReport());

        //// DISABLE THESE....
    //~UAtom() = delete;
    //UAtom(const UAtom&) = delete;
    //UAtom(UAtom&&) = delete;
    //UAtom& operator=(const UAtom&) = delete;
    //UAtom& operator=(UAtom&&) = delete;
//};

//UAtom&          uget(Atom);



    //  return TRUE if tags were added/removed
AddRem<Class>       uclasses(Atom, Document doc, const StringSet&);

Image               uicon(Atom, Image);

Vector<Property>    uproperties(Atom, Document doc, const cdb::KVReport& attributes);

    //  returns TRUE if tags were added/removed
AddRem<Tag>         utags(Atom, Document doc, const StringSet&);
