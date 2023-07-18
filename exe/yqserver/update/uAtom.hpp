////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <update/preamble.hpp>
#include <mithril/atom/Atom.hpp>
#include <mithril/attribute/Attribute.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/tag/Tag.hpp>
#include <basic/CollectionUtils.hpp>

namespace yq::mithril { 
    struct KVTree; 
    struct AtomChangeData;
    struct AtomNotifier;
}

namespace yq::mithril::update {
    struct UAtom {
        static UAtom                    get(Atom t);
        static std::pair<UAtom,bool>    create(Document);
        static bool                     triggered(const AtomChangeData&acd, const AtomNotifier& an);
        static void                     execute(const AtomChangeData&acd);
        
        static void                     changed(KVTree&, Document);
        static Atom                     s3_create(KVTree&, Document);
        static void                     s3_bind(Document);
        static void                     s3_notify(Document);

        const Atom      x;
        const id_t      id;

        UAtom(Atom);
        
        void                i_atom(KVTree&);
        void                u_atom(KVTree&);
        void                i_notify();
        
        //  ---- Abbreviation
        //! Extracts an abbreviation for the atom
        static std::string  q_abbr(const KVTree& attrs);
        
        //! Inserts the abbreviation
        void                i_abbr(const KVTree& attrs);
        
        //! Updates the abbreviation, returns TRUE if it's been changed
        bool                u_abbr(const KVTree& attrs);


        //  ---- Class

        //! Inserts classes (should be a new atom)
        void                i_classes(const ClassSet&);
        //! Inserts classes (should be a new atom)
        ClassSet            i_classes(KVTree&, bool autoDeleteKeys=true);
        
        //! Updates the atom's class set
        SetChanges<Class>   u_classes(const ClassSet&);

        //! Updates the atom's class set (from key-value tree)
        SetChanges<Class>   u_classes(KVTree&, bool autoDeleteKeys=true);
        
        //  ---- Properties
        
        Atom::Property      i_prop(Attribute);
        //static void         x_prop(Atom::Property);
        
        void                i_props(std::span<const Attribute::Diff>);
        void                u_props(std::span<const Attribute::Diff>);
        void                x_props(std::span<const Attribute::Diff>);
        
        void                i_attributes(const KVTree&);
        void                u_attributes(const KVTree&);
        


        //  ---- Tags
        
        void                i_tags(const TagSet&);
        TagSet              i_tags(KVTree&, bool autoDeleteKeys=true);
        
        SetChanges<Tag>     u_tags(const TagSet&);
        SetChanges<Tag>     u_tags(KVTree&, bool autoDeleteKeys=true);

        //  ---- Title
        
        //! Extracts a title for the atom
        //! If none is in the attributes, the key will be used
        std::string         q_title(const KVTree& attrs) const;
        
        void                i_title(const KVTree& attrs);
        
        //! Updates the title, returns TRUE if it's been changed
        bool                u_title(const KVTree& attrs);
    };
}
