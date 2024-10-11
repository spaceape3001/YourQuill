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
#include <yq/math/Counter.hpp>

//#include "Graph.hpp"

#define TBL_CLASSES             "Classes"

#define TBL_CLASS_ALIAS         "ClassAlias"
#define TBL_CLASS_DEPEND        "ClassDepend"
#define TBL_CLASS_FIELD         "ClassField"
#define TBL_CLASS_LOOKUP        "ClassLookup"
#define TBL_CLASS_PREFIX        "ClassPrefix"
#define TBL_CLASS_REVERSE       "ClassReverse"
#define TBL_CLASS_SOURCE        "ClassSource"
#define TBL_CLASS_SUFFIX        "ClassSuffix"
#define TBL_CLASS_TAG           "ClassTag"
#define TBL_CLASS_TARGET        "ClassTarget"

#define TBL_CLSDEF_ALIAS        "ClsDefAlias"
#define TBL_CLSDEF_PREFIX       "ClsDefPrefix"
#define TBL_CLSDEF_REVERSE      "ClsDefReverse"
#define TBL_CLSDEF_SOURCE       "ClsDefSource"
#define TBL_CLSDEF_SUFFIX       "ClsDefSuffix"
#define TBL_CLSDEF_TARGET       "ClsDefTarget"
#define TBL_CLSDEF_USE          "ClsDefUse"



namespace yq::mithril::cdb {
    string_set_t                aliases_set(Class);

    /*! \brief All classes in the cache database
    
        \param[in]  sorted  Yes/No to sort by key (default is no)
        \return vector of classes
    */
    ClassVector                 all_classes(Sorted sorted=Sorted{});

    ClassVector                 all_classes(Category, Sorted sorted=Sorted{});
    
    /*! \brief Count of all classes declared in the cache
    */
    size_t                      all_classes_count();
    
    
    //! Alternative keys for class
    string_set_t                alternative_keys(Class);
    
    ClassVector                 base_classes(Class, Sorted sorted=Sorted());
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
    ClassVector                 classes(const string_set_t&, bool noisy=false);
    
    
    //! Gets all classes matching the set of keys as a set
    ClassSet                    classes_set(const string_set_t&, bool noisy=false);

    //! Gets all classes matching the set of keys as a set
    ClassSet                    classes_set(const string_view_set_t&, bool noisy=false);
    
    //! Gets all classes that are tagged
    ClassVector                 classes_with_tag(Tag, Sorted sorted=Sorted{});

    //! Creates the specified class from document
    Class                       db_class(Document, bool *wasCreated=nullptr);
    
    //! Creates the specified class from key
    Class                       db_class(std::string_view, bool *wasCreated=nullptr);
    
    //! Creates all classes
    ClassVector                 db_classes(const string_view_set_t&);

    //! Creates all classes
    ClassVector                 db_classes(const string_set_t&);
    
    string_set_t                def_alias_set(Class);
    
    string_set_t                def_prefix_set(Class);

    ClassSet                    def_reverse_set(Class);
    
    ClassSet                    def_rev_reverse_set(Class);

    //! All classes that "used" this class
    ClassVector                 def_rev_use(Class, Sorted sorted=Sorted{});
    
    //! All classes that "used" this class (as a set)
    ClassSet                    def_rev_use_set(Class);
    
    //! Count of classes "used" this class
    uint64_t                    def_rev_use_count(Class);

    ClassSet                    def_source_set(Class);

    string_set_t                def_suffix_set(Class);

    ClassSet                    def_target_set(Class);

    //! All classes that were defined "use" for this class
    ClassVector                 def_use(Class, Sorted sorted=Sorted{});
    
    //! All classes that were defined "use" for this class (as a set)
    ClassSet                    def_use_set(Class);
    
    //! Count of classes that were defined "use" for this class
    uint64_t                    def_use_count(Class);
    
    Graphviz                    dep_graph(Class);
    

    //! all derives for this class
    ClassVector                 derived_classes(Class, Sorted sorted=Sorted());
    
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
    
    ClassVector                 edge_classes_in(Class, Sorted sorted=Sorted());
    ClassVector                 edge_classes_out(Class, Sorted sorted=Sorted());
  
  
    bool                        exists(Class);

    bool                        exists_class(uint64_t);
    Field                       field(Class, std::string_view);
    FieldVector                 fields(Class, Sorted sorted=Sorted());
    size_t                      fields_count(Class);

    Image                       icon(Class);
    
    //AtomVector            inbound(Atom);
    ClassVector                 inbound_classes(Class, Sorted sorted=Sorted());
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
    
    //AtomVector            outbound(Atom);
    ClassVector                 outbound_classes(Class, Sorted sorted=Sorted());
    std::vector<Class::Rank>    outbound_classes_ranked(Class, Sorted sorted=Sorted());
    
    std::string                 plural(Class);
    

    //! \brief Returns the FIRST class fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    Class::SharedFile           read(Class, const RootDir*, cdb_options_t opts=0);

    std::vector<ClassFragDoc>   reads(Class, cdb_options_t opts=0);
    std::vector<ClassFragDoc>   reads(Class, class RootDir*, cdb_options_t opts=0);

    //ClassSet             rev_use_set(Class);
    //ClassVector          rev_uses(Class, Sorted sorted=Sorted());
    //size_t                      rev_uses_count(Class);

    ClassVector                 reverse_classes(Class, Sorted sorted=Sorted());
    size_t                      reverse_classes_count(Class c);
    std::vector<Class::Rank>    reverse_classes_ranked(Class, Sorted sorted=Sorted());
    std::vector<Class::Rank>    reverse_classes_ranked_limited(Class, uint64_t, Sorted sorted=Sorted());
    
    ClassVector                 source_classes(Class, Sorted sorted=Sorted());
    size_t                      source_classes_count(Class);
    std::vector<Class::Rank>    source_classes_ranked(Class, Sorted sorted=Sorted());
    std::vector<Class::Rank>    source_classes_ranked_limited(Class, uint64_t, Sorted sorted=Sorted());
    
    
    TagVector                   tags(Class, Sorted sorted=Sorted());
    size_t                      tags_count(Class);
    TagSet                      tags_set(Class);

    ClassVector                 target_classes(Class, Sorted sorted=Sorted());
    size_t                      target_classes_count(Class);
    std::vector<Class::Rank>    target_classes_ranked(Class, Sorted sorted=Sorted());
    std::vector<Class::Rank>    target_classes_ranked_limited(Class, uint64_t, Sorted sorted=Sorted());

    //! Returns the URL path for the class type
    std::string                 url(Class);


    //! All classes that were listed in the file
    //ClassSet             use_set(Class);
    //ClassVector          uses(Class, Sorted sorted=Sorted());
    //size_t                      uses_count(Class);

    
    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    Class::SharedFile           writable(Class, const RootDir*, cdb_options_t opts=0);
}
