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
#include <0/basic/CollectionUtils.hpp>

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

        //! Inserts into atoms attributes (ie, leaf)
        //! \note YES, reference, assume the tree is trashed on return
        static void                 i_atom(Atom, KVTree&, Class c=Class(), bool notify=false);
        static void                 i_notify(Atom, bool recursive=false);

        //! Updates atoms attributes
        //! \note YES, reference, assume the tree is trashed on return
        static void                 u_atom(Atom, KVTree&, Class c=Class());
        static void                 u_icon(Atom, Image);
        
        static void                 x_atom(Atom);

        UAtom(Atom);
        

    private:
        static bool                 triggered(const AtomChangeData&acd, const AtomNotifier& an);
        static void                 execute(const AtomChangeData&acd);


        void                        i_abbr(KVTree& attrs);
        void                        i_classes(const ClassSet&);
        void                        i_classes(KVTree&attrs, Class c=Class());
        void                        i_prop(Attribute::Diff&);
        void                        i_props(std::span<Attribute::Diff>);
        void                        i_tags(const TagSet& tags);
        void                        i_tags(KVTree&attrs);
        void                        i_title(KVTree& attrs);
        
        std::string                 q_abbr(KVTree&) const;
        std::string                 q_title(KVTree&) const;
        
        bool                        u_abbr(KVTree& attrs);
        SetChanges<Class>           u_classes(const ClassSet&);
        SetChanges<Class>           u_classes(KVTree&, Class c=Class());
        void                        u_prop(Attribute::Diff&);
        void                        u_props(std::span<Attribute::Diff>);
        void                        u_resolve();
        SetChanges<Tag>             u_tags(const TagSet&);
        SetChanges<Tag>             u_tags(KVTree&);
        bool                        u_title(KVTree& attrs);

        void                        x_prop(Attribute::Diff&);
        void                        x_props(Attribute);
        void                        x_props(std::span<Attribute::Diff>);

        const Atom                  m_atom;
        const id_t                  m_id;
        ClassSet                    m_classes;
        ResolveMap                  m_resolve;
        bool                        m_resolved = false;
    };
}
