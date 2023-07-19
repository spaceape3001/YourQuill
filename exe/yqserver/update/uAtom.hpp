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
    struct Image;
}

namespace yq::mithril::update {
    class UAtom {
    public:
        //static UAtom                    get(Atom t);
        //static std::pair<UAtom,bool>    create(Document);
        
        //static void                     changed(KVTree&, Document);
        //static Atom                     s3_create(KVTree&, Document);
        //static void                     s3_bind(Document);
        //static void                     s3_notify(Document);
        
        //! final S4 for atoms, converting things into edges, etc
        static void                 s4();

        static void                 i_atom(Atom, KVTree&&, Class c=Class(), bool notify=false);
        static void                 i_notify(Atom, bool recursive=false);

        static void                 u_atom(Atom, KVTree&&, Class c=Class());
        static void                 u_icon(Atom, Image);
        
        static void                 x_atom(Atom);

    private:
        static bool                 triggered(const AtomChangeData&acd, const AtomNotifier& an);
        static void                 execute(const AtomChangeData&acd);


        static void                 i_abbr(Atom, KVTree& attrs);
        static void                 i_classes(Atom, const ClassSet&);
        static ClassSet             i_classes(Atom, KVTree&attrs, Class c=Class());
        static Atom::Property       i_prop(Atom, Attribute::Diff&);
        static void                 i_props(Atom, std::span<Attribute::Diff>);
        static void                 i_tags(Atom, const TagSet& tags);
        static TagSet               i_tags(Atom, KVTree&attrs);
        static void                 i_title(Atom, KVTree& attrs);
        
        static std::string          q_abbr(Atom, KVTree&);
        static std::string          q_title(Atom, KVTree&);
        
        static bool                 u_abbr(Atom, KVTree& attrs);
        static SetChanges<Class>    u_classes(Atom, const ClassSet&);
        static SetChanges<Class>    u_classes(Atom, KVTree&, Class c=Class());
        static void                 u_prop(Atom, Attribute::Diff&);
        static void                 u_props(Atom, std::span<Attribute::Diff>);
        static SetChanges<Tag>      u_tags(Atom, const TagSet&);
        static SetChanges<Tag>      u_tags(Atom, KVTree&);
        static bool                 u_title(Atom, KVTree& attrs);

        static void                 x_prop(Atom, Attribute::Diff&);
        static void                 x_props(Atom, Attribute);
        static void                 x_props(Atom, std::span<Attribute::Diff>);

    };
}
