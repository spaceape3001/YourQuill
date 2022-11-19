////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/preamble.hpp>
#include <kernel/agw/ClassInfo.hpp>
#include <kernel/agw/ClassFile.hpp>
#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Sorted.hpp>
#include <kernel/file/Document.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/org/Category.hpp>
#include <math/Counter.hpp>

//#include "Graph.hpp"


namespace yq {
    namespace agw {
        using ClassFragDoc          = std::pair<Fragment, Class::SharedFile>;
    }


    namespace cdb {
        

        /*! \brief All classes in the cache database
        
            \param[in]  sorted  Yes/No to sort by key (default is no)
            \return vector of classes
        */
        agw::ClassVector            all_classes(Sorted sorted=Sorted{});
        
        /*! \brief Count of all classes declared in the cache
        */
        size_t                          all_classes_count();
        
        //! Alternative keys for class
        string_set_t                    alternative_keys(agw::Class);
        
        agw::ClassVector                base_classes(agw::Class, Sorted sorted=Sorted());
        size_t                          base_classes_count(agw::Class);
        std::vector<agw::Class::Rank>   base_classes_ranked(agw::Class, Sorted sorted=Sorted());
        std::vector<agw::Class::Rank>   base_classes_ranked_limited(agw::Class, uint64_t, Sorted sorted=Sorted());
        agw::ClassCountMap              base_classes_ranked_merged_map(const agw::ClassSet&, bool inSet=false);

        //! Binding for class?
        std::string                     binding(agw::Class);
        
        //! Brief for class
        std::string                     brief(agw::Class);
        
        //! Category for class
        Category                        category(agw::Class);

        //! Gets class by ID
        agw::Class                      class_(uint64_t);
        
        //! Gets class by defining document
        agw::Class                      class_(Document, bool calc=false);
        
        //! Gets class by key
        agw::Class                      class_(std::string_view );

        //! Gets the specified class fragment as data
        agw::Class::SharedFile          class_doc(Fragment, cdb_options_t opts=0);
        
        //! Gets all classes matching the set of keys
        agw::ClassVector                classes(const string_set_t&, bool noisy=false);
        
        
        //! Gets all classes matching the set of keys as a set
        std::set<agw::Class>            classes_set(const string_set_t&, bool noisy=false);

        //! Gets all classes matching the set of keys as a set
        std::set<agw::Class>            classes_set(const string_view_set_t&, bool noisy=false);
        
        //! Gets all classes that are tagged
        agw::ClassVector                classes_with_tag(Tag, Sorted sorted=Sorted{});

        //! Creates the specified class from document
        agw::Class                      db_class(Document, bool *wasCreated=nullptr);
        
        //! Creates the specified class from key
        agw::Class                      db_class(std::string_view, bool *wasCreated=nullptr);
        
        //! Creates all classes
        agw::ClassVector                db_classes(const string_view_set_t&);

        //! Creates all classes
        agw::ClassVector                db_classes(const string_set_t&);
        
        //! Defined aliases for class
        //string_set_t                def_aliases(agw::Class);
        
        //! Defined derived classes for class
        //agw::ClassVector          def_derived(agw::Class, Sorted sorted=Sorted());
        
        //! Defined fields for class
        //agw::FieldVector          def_fields(agw::Class c, Sorted sorted=Sorted{});
        
        //! Defined reverses for class
        //agw::ClassVector          def_reverse(agw::Class, Sorted sorted=Sorted());
        
        //! Defined prefixes for class
        //string_set_t                def_prefixes(agw::Class);
        
        //! Defined sources for class
        //agw::ClassVector          def_source(agw::Class, Sorted sorted=Sorted());
        
        //! Defined suffixes for class
        //string_set_t                def_suffixes(agw::Class);
        
        //! Defined targets for class
        //agw::ClassVector          def_target(agw::Class, Sorted sorted=Sorted());
        
        //! Defined use (ie bases) for class
        //agw::ClassVector          def_use(agw::Class, Sorted sorted=Sorted());
        
        //Graph                   dep_graph(agw::Class);

        //! all derives for this class
        agw::ClassVector                derived_classes(agw::Class, Sorted sorted=Sorted());

                //  Rank here is number of hops
        std::vector<agw::Class::Rank>   derived_classes_ranked(agw::Class, Sorted sorted=Sorted());

                //  Rank here is number of hops
        std::vector<agw::Class::Rank>   derived_classes_ranked_limited(agw::Class, uint64_t maxDepth, Sorted sorted=Sorted());
        
            // Rank is the number of hops
        agw::ClassCountMap              derived_classes_ranked_merged_map(const agw::ClassSet&, bool inSet=false);

        //Document                        document(Atom);
        Document                        document(agw::Class);
        
        agw::ClassVector                edge_classes_in(agw::Class, Sorted sorted=Sorted());
        agw::ClassVector                edge_classes_out(agw::Class, Sorted sorted=Sorted());
      
      
        bool                            exists(agw::Class);

        bool                            exists_class(uint64_t);
        agw::Field                      field(agw::Class, std::string_view);
        agw::FieldVector                fields(agw::Class, Sorted sorted=Sorted());
        size_t                          fields_count(agw::Class);

        Image                           icon(agw::Class);
        
        //std::vector<Atom>            inbound(Atom);
        agw::ClassVector                inbound_classes(agw::Class, Sorted sorted=Sorted());
        std::vector<agw::Class::Rank>   inbound_classes_ranked(agw::Class, Sorted sorted=Sorted());

        agw::Class::Info                info(agw::Class, bool autoKeyToName=false);

        bool                            is(agw::Class classInQuestion, agw::Class base);
        bool                            is_all(agw::Class, std::initializer_list<agw::Class>);
        bool                            is_any(agw::Class, std::initializer_list<agw::Class>);

        bool                            is_edge(agw::Class);
        //bool                          is_tagged(agw::Class, Tag);

        std::string                     key(agw::Class);
        std::string                     label(agw::Class);

        agw::Class::SharedData          merged(agw::Class, cdb_options_t opts=0);
        agw::Class                      make_class(std::string_view, const Root* rt=nullptr, cdb_options_t opts=0, bool *wasCreated=nullptr);
        
        agw::ClassCountMap              make_count_map(const std::vector<agw::Class::Rank>&);

        std::string                     name(agw::Class);
        
        NKI                             nki(agw::Class, bool autoKeyToName=false);
        
        //std::vector<Atom>               outbound(Atom);
        agw::ClassVector                outbound_classes(agw::Class, Sorted sorted=Sorted());
        std::vector<agw::Class::Rank>   outbound_classes_ranked(agw::Class, Sorted sorted=Sorted());
        
        std::string                     plural(agw::Class);
        

        //! \brief Returns the FIRST class fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        agw::Class::SharedFile          read(agw::Class, const Root*, cdb_options_t opts=0);

        std::vector<agw::ClassFragDoc>  reads(agw::Class, cdb_options_t opts=0);
        std::vector<agw::ClassFragDoc>  reads(agw::Class, class Root*, cdb_options_t opts=0);

        agw::ClassVector                reverses(agw::Class, Sorted sorted=Sorted());
        
        agw::ClassVector                source_classes(agw::Class, Sorted sorted=Sorted());
        size_t                          source_classes_count(agw::Class);
        std::vector<agw::Class::Rank>   source_classes_ranked(agw::Class, Sorted sorted=Sorted());
        std::vector<agw::Class::Rank>   source_classes_ranked_limited(agw::Class, uint64_t, Sorted sorted=Sorted());
        
        
        std::set<Tag>                   tag_set(agw::Class);
        std::vector<Tag>                tags(agw::Class, Sorted sorted=Sorted());
        size_t                          tags_count(agw::Class);

        agw::ClassVector                target_classes(agw::Class, Sorted sorted=Sorted());
        size_t                          target_classes_count(agw::Class);



        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        agw::Class::SharedFile           writable(agw::Class, const Root*, cdb_options_t opts=0);
    }
}
