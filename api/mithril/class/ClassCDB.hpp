////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/class/ClassInfo.hpp>
#include <mithril/class/ClassFile.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/category/Category.hpp>
#include <0/math/Counter.hpp>

//#include "Graph.hpp"


namespace yq::mithril::cdb {
    string_set_t                aliases(Class);

    /*! \brief All classes in the cache database
    
        \param[in]  sorted  Yes/No to sort by key (default is no)
        \return vector of classes
    */
    std::vector<Class>          all_classes(Sorted sorted=Sorted{});
    
    /*! \brief Count of all classes declared in the cache
    */
    size_t                      all_classes_count();
    
    
    //! Alternative keys for class
    string_set_t                alternative_keys(Class);
    
    std::vector<Class>          base_classes(Class, Sorted sorted=Sorted());
    size_t                      base_classes_count(Class);
    std::vector<Class::Rank>    base_classes_ranked(Class, Sorted sorted=Sorted());
    std::vector<Class::Rank>    base_classes_ranked_limited(Class, uint64_t, Sorted sorted=Sorted());
    ClassCountMap               base_classes_ranked_merged_map(const ClassSet&, bool inSet=false);
    
    ClassHopMap                 base_hops(Class);

    //! Binding for class?
    std::string                 binding(Class);
    
    //! Brief for class
    std::string                 brief(Class);
    
    //! Category for class
    Category                    category(Class);

    //! Gets class by ID
    Class                       class_(uint64_t);
    
    //! Gets class by defining document
    Class                       class_(Document, bool calc=false);
    
    //! Gets class by key
    Class                       class_(std::string_view );

    //! Gets the specified class fragment as data
    Class::SharedFile           class_doc(Fragment, cdb_options_t opts=0);
    
    //! Gets all classes matching the set of keys
    std::vector<Class>          classes(const string_set_t&, bool noisy=false);
    
    
    //! Gets all classes matching the set of keys as a set
    std::set<Class>             classes_set(const string_set_t&, bool noisy=false);

    //! Gets all classes matching the set of keys as a set
    std::set<Class>             classes_set(const string_view_set_t&, bool noisy=false);
    
    //! Gets all classes that are tagged
    std::vector<Class>          classes_with_tag(Tag, Sorted sorted=Sorted{});

    //! Creates the specified class from document
    Class                       db_class(Document, bool *wasCreated=nullptr);
    
    //! Creates the specified class from key
    Class                       db_class(std::string_view, bool *wasCreated=nullptr);
    
    //! Creates all classes
    std::vector<Class>          db_classes(const string_view_set_t&);

    //! Creates all classes
    std::vector<Class>          db_classes(const string_set_t&);
    
    
    Graphviz                    dep_graph(Class);

    //! all derives for this class
    std::vector<Class>          derived_classes(Class, Sorted sorted=Sorted());
    
    ClassHopMap                 derived_hops(Class);

            //  Rank here is number of hops
    std::vector<Class::Rank>    derived_classes_ranked(Class, Sorted sorted=Sorted());

            //  Rank here is number of hops
    std::vector<Class::Rank>    derived_classes_ranked_limited(Class, uint64_t maxDepth, Sorted sorted=Sorted());
    
        // Rank is the number of hops
    ClassCountMap               derived_classes_ranked_merged_map(const ClassSet&, bool inSet=false);

    //! Dev url
    std::string                 dev_url(Class);

    //Document                document(Atom);
    Document                    document(Class);
    
    std::vector<Class>          edge_classes_in(Class, Sorted sorted=Sorted());
    std::vector<Class>          edge_classes_out(Class, Sorted sorted=Sorted());
  
  
    bool                        exists(Class);

    bool                        exists_class(uint64_t);
    Field                       field(Class, std::string_view);
    std::vector<Field>          fields(Class, Sorted sorted=Sorted());
    size_t                      fields_count(Class);

    Image                       icon(Class);
    
    //std::vector<Atom>            inbound(Atom);
    std::vector<Class>          inbound_classes(Class, Sorted sorted=Sorted());
    std::vector<Class::Rank>    inbound_classes_ranked(Class, Sorted sorted=Sorted());

    Class::Info                 info(Class, bool autoKeyToName=false);

    bool                        is(Class classInQuestion, Class base);
    bool                        is_all(Class, std::initializer_list<Class>);
    bool                        is_any(Class, std::initializer_list<Class>);

    bool                        is_edge(Class);
    //bool                    is_tagged(Class, Tag);

    std::string                 key(Class);
    std::string                 label(Class);

    Class::SharedData           merged(Class, cdb_options_t opts=0);
    Class                       make_class(std::string_view, const RootDir* rt=nullptr, cdb_options_t opts=0, bool *wasCreated=nullptr);
    
    ClassCountMap               make_count_map(const std::vector<Class::Rank>&);

    std::string                 name(Class);
    
    NKI                         nki(Class, bool autoKeyToName=false);
    
    //std::vector<Atom>            outbound(Atom);
    std::vector<Class>          outbound_classes(Class, Sorted sorted=Sorted());
    std::vector<Class::Rank>    outbound_classes_ranked(Class, Sorted sorted=Sorted());
    
    std::string                 plural(Class);
    

    //! \brief Returns the FIRST class fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    Class::SharedFile           read(Class, const RootDir*, cdb_options_t opts=0);

    std::vector<ClassFragDoc>   reads(Class, cdb_options_t opts=0);
    std::vector<ClassFragDoc>   reads(Class, class RootDir*, cdb_options_t opts=0);

    std::set<Class>             rev_use_set(Class);
    std::vector<Class>          rev_uses(Class, Sorted sorted=Sorted());
    size_t                      rev_uses_count(Class);

    std::vector<Class>          reverse_classes(Class, Sorted sorted=Sorted());
    size_t                      reverse_classes_count(Class c);
    std::vector<Class::Rank>    reverse_classes_ranked(Class, Sorted sorted=Sorted());
    std::vector<Class::Rank>    reverse_classes_ranked_limited(Class, uint64_t, Sorted sorted=Sorted());
    
    std::vector<Class>          source_classes(Class, Sorted sorted=Sorted());
    size_t                      source_classes_count(Class);
    std::vector<Class::Rank>    source_classes_ranked(Class, Sorted sorted=Sorted());
    std::vector<Class::Rank>    source_classes_ranked_limited(Class, uint64_t, Sorted sorted=Sorted());
    
    
    std::set<Tag>               tag_set(Class);
    std::vector<Tag>            tags(Class, Sorted sorted=Sorted());
    size_t                      tags_count(Class);

    std::vector<Class>          target_classes(Class, Sorted sorted=Sorted());
    size_t                      target_classes_count(Class);
    std::vector<Class::Rank>    target_classes_ranked(Class, Sorted sorted=Sorted());
    std::vector<Class::Rank>    target_classes_ranked_limited(Class, uint64_t, Sorted sorted=Sorted());

    //! Returns the URL path for the class type
    std::string                 url(Class);


    //! All classes that were listed in the file
    std::set<Class>             use_set(Class);
    std::vector<Class>          uses(Class, Sorted sorted=Sorted());
    size_t                      uses_count(Class);

    
    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    Class::SharedFile           writable(Class, const RootDir*, cdb_options_t opts=0);
}
