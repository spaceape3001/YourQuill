////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <update/preamble.hpp>
#include <mithril/class/ClassData.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/field/Field.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/tag/Tag.hpp>

namespace yq::mithril::update {
    struct UClass : public U<Class> {
        static UClass&      get(Class);
        static std::pair<UClass&, bool>  create(Document);
        
        static void             notify(Fragment,Change);
        static void             icons(Fragment,Change);
        
        //! Create the class in stage3 with basics (tags, aliases)
        static void             s3(Document);
        
        //! Bind the immediate classes (ie, use, targets, sources)
        static void             s3_bind(Document);
        
        //! Extend the use to base/derived
        static void             s3_extend(Document);
        
        //! Deduce all target, sources, fields, etc
        static void             s3_deduce(Document);


        using Resolve       = std::variant<std::monostate,Class,Field>;
        using ResolveMap    = std::map<std::string,Resolve,IgCase>;

        const Document      doc;

        Class::SharedData   def;
        ResolveMap          resolve;
        Image               icon;
        TagSet              tags;
        string_set_t        aliases;
        ClassSet            use;
        ClassHopMap         bases;
        ClassHopMap         derives;
        ClassHopMap         targets;    // target (node) classes
        ClassHopMap         inbounds;   // edge classes that can be inbound
        ClassHopMap         sources;    // source (node) classes
        ClassHopMap         outbounds;  // edge classes that can be outbound
        ClassHopMap         reverses;   // classes that are reverse of this edge
        FieldHopMap         fields;     // field hops
        FieldSet            fields_direct;   // fields from direct
        FieldSet            fields_all;      // fields from indirect (ALL)
        
        UClass(Class);
        
        void                reload();
        void                u_alias();
        void                u_info();
        void                u_icon();
        void                u_tags();
        void                u_fields();
        
        void                x_erase();
        
        //!     Initial seeding of dependencies (startup only)
        void                i0_uses();
        
        //!     Initial seeding of targets (startup only)
        void                i0_targets();
        
        //!     Initial seeding of sources (startup only)
        void                i0_sources();
        
        //!     Initial seeding of reverses (startup only)
        void                i0_reverses();
        
        //!     Deducing ALL bases/derives...
        void                i1_bases();
        void                i1_bases(Class,hop_t);
        
        string_set_t        enum_aliases() const;
        FieldHopMap         enum_fields() const;
        
        
        //void                flash(FF);

    #if 0    
        StringCountMap  alias;
        StringCountMap  prefix;
        ClassCountMap   reverse;
        ClassCountMap   source;
        StringCountMap  suffix;
        ClassCountMap   target;
    #endif

    };
}
