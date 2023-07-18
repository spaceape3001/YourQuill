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
        
        //! Binds base classes
        static void             s3_extend(Document);

        //! Binds derive classes
        static void             s3_derives(Document);
        
        //! Deduce all target, sources, fields, etc
        static void             s3_deduce(Document);
        
        //! Maybe final pass, propagate to the remaining...
        static void             s3_propagate(Document);
        
        struct Outbound { Class cls; };
        struct Node { Class cls; };

        using Resolve       = std::variant<std::monostate,Node,Field,Outbound>;

        //  ResolveMap *might* become multimap
        using ResolveMap    = std::map<std::string,Resolve,IgCase>;

        const Document      doc;

        Class::SharedData   def;
        ResolveMap          resolve;
        Image               icon;
        TagSet              tags;
        string_set_t        aliases;
        DD<Class>           bases;
        DD<Class>           derives;
        DD<Class>           targets;
        DD<Class>           sources;
        DD<Class>           inbounds;
        DD<Class>           outbounds;
        std::string_view    binding;
        bool                isEdge          = false;
        
        //  This is what can reverses *this* class
        DD<Class>           reverses;
        DD<Field>           fields;
        
        UClass(Class);
        
        void                reload();
        void                u_alias();
        void                u_bases();
        void                u_derives();
        void                u_edge();
        void                u_fields();
        void                u_icon();
        void                u_inbounds();
        void                u_info();
        void                u_outbounds();
        void                u_reverses();
        void                u_resolves();
        void                u_sources();
        void                u_tags();
        void                u_targets();
        
        void                x_erase();
        
        //!     Initial seeding of dependencies (startup only)
        void                i0_uses();
        
        //!     Initial seeding of targets (startup only)
        void                i0_targets();
        
        //!     Initial seeding of sources (startup only)
        void                i0_sources();
        
        //!     Initial seeding of reverses (startup only)
        void                i0_reverses();
        
        string_set_t        enum_aliases() const;
        ClassHopMap         enum_bases() const;
        FieldHopMap         enum_fields() const;
        ClassHopMap         enum_reverses() const;
        ClassHopMap         enum_sources() const;
        ClassHopMap         enum_targets() const;
        
        static void         enum_base(ClassHopMap&, Class, hop_t);
        
        
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
