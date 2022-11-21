////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/preamble.hpp>
#include <kernel/Category.hpp>
#include <kernel/ClassInfo.hpp>
#include <kernel/ClassFile.hpp>
#include <kernel/CacheFwd.hpp>
#include <kernel/Document.hpp>
#include <kernel/Image.hpp>
#include <math/Counter.hpp>

//#include "Graph.hpp"


namespace yq {
    using ClassFragDoc          = std::pair<Fragment, Class::SharedFile>;
    using ClassFragDocVector    = std::vector<ClassFragDoc>;

    namespace cdb {
        

        /*! \brief All classes in the cache database
        
            \param[in]  sorted  Yes/No to sort by key (default is no)
            \return vector of classes
        */
        ClassVector            all_classes(Sorted sorted=Sorted{});
        
        /*! \brief Count of all classes declared in the cache
        */
        size_t                          all_classes_count();
        
        //! Alternative keys for class
        string_set_t                    alternative_keys(Class);
        
        ClassVector                base_classes(Class, Sorted sorted=Sorted());
        size_t                          base_classes_count(Class);
        std::vector<Class::Rank>   base_classes_ranked(Class, Sorted sorted=Sorted());
        std::vector<Class::Rank>   base_classes_ranked_limited(Class, uint64_t, Sorted sorted=Sorted());
        ClassCountMap              base_classes_ranked_merged_map(const ClassSet&, bool inSet=false);

        //! Binding for class?
        std::string                     binding(Class);
        
        //! Brief for class
        std::string                     brief(Class);
        
        //! Category for class
        Category                   category(Class);

        //! Gets class by ID
        Class                      class_(uint64_t);
        
        //! Gets class by defining document
        Class                      class_(Document, bool calc=false);
        
        //! Gets class by key
        Class                      class_(std::string_view );

        //! Gets the specified class fragment as data
        Class::SharedFile          class_doc(Fragment, cdb_options_t opts=0);
        
        //! Gets all classes matching the set of keys
        ClassVector                classes(const string_set_t&, bool noisy=false);
        
        
        //! Gets all classes matching the set of keys as a set
        std::set<Class>            classes_set(const string_set_t&, bool noisy=false);

        //! Gets all classes matching the set of keys as a set
        std::set<Class>            classes_set(const string_view_set_t&, bool noisy=false);
        
        //! Gets all classes that are tagged
        ClassVector                classes_with_tag(Tag, Sorted sorted=Sorted{});

        //! Creates the specified class from document
        Class                      db_class(Document, bool *wasCreated=nullptr);
        
        //! Creates the specified class from key
        Class                      db_class(std::string_view, bool *wasCreated=nullptr);
        
        //! Creates all classes
        ClassVector                db_classes(const string_view_set_t&);

        //! Creates all classes
        ClassVector                db_classes(const string_set_t&);
        
        //! Defined aliases for class
        //string_set_t                def_aliases(Class);
        
        //! Defined derived classes for class
        //ClassVector          def_derived(Class, Sorted sorted=Sorted());
        
        //! Defined fields for class
        //FieldVector          def_fields(Class c, Sorted sorted=Sorted{});
        
        //! Defined reverses for class
        //ClassVector          def_reverse(Class, Sorted sorted=Sorted());
        
        //! Defined prefixes for class
        //string_set_t                def_prefixes(Class);
        
        //! Defined sources for class
        //ClassVector          def_source(Class, Sorted sorted=Sorted());
        
        //! Defined suffixes for class
        //string_set_t                def_suffixes(Class);
        
        //! Defined targets for class
        //ClassVector          def_target(Class, Sorted sorted=Sorted());
        
        //! Defined use (ie bases) for class
        //ClassVector          def_use(Class, Sorted sorted=Sorted());
        
        //Graph                   dep_graph(Class);

        //! all derives for this class
        ClassVector                derived_classes(Class, Sorted sorted=Sorted());

                //  Rank here is number of hops
        std::vector<Class::Rank>   derived_classes_ranked(Class, Sorted sorted=Sorted());

                //  Rank here is number of hops
        std::vector<Class::Rank>   derived_classes_ranked_limited(Class, uint64_t maxDepth, Sorted sorted=Sorted());
        
            // Rank is the number of hops
        ClassCountMap              derived_classes_ranked_merged_map(const ClassSet&, bool inSet=false);

        //Document                        document(Atom);
        Document                  document(Class);
        
        ClassVector                edge_classes_in(Class, Sorted sorted=Sorted());
        ClassVector                edge_classes_out(Class, Sorted sorted=Sorted());
      
      
        bool                            exists(Class);

        bool                            exists_class(uint64_t);
        Field                      field(Class, std::string_view);
        FieldVector                fields(Class, Sorted sorted=Sorted());
        size_t                          fields_count(Class);

        Image                           icon(Class);
        
        //std::vector<Atom>            inbound(Atom);
        ClassVector                inbound_classes(Class, Sorted sorted=Sorted());
        std::vector<Class::Rank>   inbound_classes_ranked(Class, Sorted sorted=Sorted());

        Class::Info                info(Class, bool autoKeyToName=false);

        bool                            is(Class classInQuestion, Class base);
        bool                            is_all(Class, std::initializer_list<Class>);
        bool                            is_any(Class, std::initializer_list<Class>);

        bool                            is_edge(Class);
        //bool                          is_tagged(Class, Tag);

        std::string                     key(Class);
        std::string                     label(Class);

        Class::SharedData          merged(Class, cdb_options_t opts=0);
        Class                      make_class(std::string_view, const Root* rt=nullptr, cdb_options_t opts=0, bool *wasCreated=nullptr);
        
        ClassCountMap              make_count_map(const std::vector<Class::Rank>&);

        std::string                     name(Class);
        
        NKI                             nki(Class, bool autoKeyToName=false);
        
        //std::vector<Atom>               outbound(Atom);
        ClassVector                outbound_classes(Class, Sorted sorted=Sorted());
        std::vector<Class::Rank>   outbound_classes_ranked(Class, Sorted sorted=Sorted());
        
        std::string                     plural(Class);
        

        //! \brief Returns the FIRST class fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Class::SharedFile          read(Class, const Root*, cdb_options_t opts=0);

        ClassFragDocVector         reads(Class, cdb_options_t opts=0);
        ClassFragDocVector         reads(Class, class Root*, cdb_options_t opts=0);

        ClassVector                reverses(Class, Sorted sorted=Sorted());
        
        ClassVector                source_classes(Class, Sorted sorted=Sorted());
        size_t                          source_classes_count(Class);
        std::vector<Class::Rank>   source_classes_ranked(Class, Sorted sorted=Sorted());
        std::vector<Class::Rank>   source_classes_ranked_limited(Class, uint64_t, Sorted sorted=Sorted());
        
        
        TagSet                     tag_set(Class);
        TagVector                  tags(Class, Sorted sorted=Sorted());
        size_t                          tags_count(Class);

        ClassVector                target_classes(Class, Sorted sorted=Sorted());
        size_t                          target_classes_count(Class);



        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Class::SharedFile           writable(Class, const Root*, cdb_options_t opts=0);
    }
}
