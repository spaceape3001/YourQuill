////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CacheFwd.hpp"
#include "ClassFile.hpp"
#include "FileSys.hpp"
#include "Graph.hpp"
#include "Image.hpp"



/*! \brief Class in the cache

    This structure represents an atom classification in the cache
*/
struct Class {
    static constexpr const char*    szExtension = "cls";
    struct Info;
    using FragDoc       = std::pair<Fragment, ClassFile::Shared>;

    static bool less_key(Class a, Class b);
    static bool less_label(Class a, Class b);
    static bool less_name(Class a, Class b);

    uint64_t  id  = 0ULL;
    constexpr auto    operator<=>(const Class&rhs) const = default;
    constexpr operator bool() const { return id != 0ULL; }
};


/*! \brief Field of the class

    A field is a pre-declared attribute for any atom using the specified class.  It's recommended to keep fields
    unqiue.
*/
struct Field {
    struct Info;
    uint64_t  id  = 0ULL;
    constexpr auto    operator<=>(const Field&rhs) const = default;
    constexpr operator bool() const { return id != 0ULL; }
};


struct Class::Info {
    String      key;
    Graph       deps;
    Document    doc;
    Image       icon;
    String      name;
    String      plural;
    String      brief;
    bool        edge    = false;

    bool operator==(const Info&) const = default;
};

struct Field::Info {
    Class       class_;
    String      brief;
    String      key, pkey;
    String      name, plural;
    bool operator==(const Info&) const = default;
};

namespace cdb {
    
    StringSet               aliases(Field);

    /*! \brief All classes in the cache database
    
        \param[in]  sorted  Yes/No to sort by key (default is no)
        \return vector of classes
    */
    ClassVec                all_classes(Sorted sorted=Sorted{});
    
    /*! \brief Count of all classes declared in the cache
    */
    size_t                  all_classes_count();


    FieldVec                all_fields(Sorted sorted=Sorted());
    size_t                  all_fields_count();

    String                  brief(Class);
    String                  brief(Field);

    Class                   class_(Field);
    Class                   class_(uint64_t);
    Class                   class_(const String&);

    ClassFile::Shared       class_doc(Fragment, bool fAllowEmpty=false);
    
    ClassVec                classes(Field, Sorted sorted=Sorted());
    ClassVec                classes(const StringSet&);
    
    
    StringSet               data_types(Field);
        
    Class                   db_class(const String&, bool *wasCreated=nullptr);
    Class                   db_class(Document, bool *wasCreated=nullptr);
    ClassVec                db_classes(const StringSet&);
    
    Field                   db_field(Class c, const String&k, bool *wasCreated=nullptr);
    //Field                   db_field(const String&, bool *wasCreated=nullptr);
    //Field                   db_field(Document, bool *wasCreated=nullptr);
    
    ClassVec                def_derived(Class, Sorted sorted=Sorted());
    FieldVec                def_fields(Class, Sorted sorted=Sorted());
    ClassVec                def_reverse(Class, Sorted sorted=Sorted());
    ClassVec                def_source(Class, Sorted sorted=Sorted());
    ClassVec                def_target(Class, Sorted sorted=Sorted());
    ClassVec                def_use(Class, Sorted sorted=Sorted());
    
    Graph                   dep_graph(Class);
    Folder                  detail_folder(Class);

    Vector<Class>           dependents(Class, Sorted sorted=Sorted());
    
    //Document                document(Atom);
    Document                document(Class);
    Document                document(Field);

    bool                    edge(Class);
    
    ClassVec                edges_in(Class, Sorted sorted=Sorted());
    ClassVec                edges_out(Class, Sorted sorted=Sorted());
  
    bool                    exists(Class);
    bool                    exists(Field);

    bool                    exists_class(uint64_t);
    bool                    exists_field(uint64_t);

    Field                   field(uint64_t);
    Field                   field(Class, const String&);
    FieldVec                fields(Class, Sorted sorted=Sorted());
    size_t                  fields_count(Class);

    Image                   icon(Class);
    Image                   icon(Field);
    
    //Vector<Atom>            inbound(Atom);
    ClassVec                inbound(Class, Sorted sorted=Sorted());

    Class::Info             info(Class, bool autoKeyToName=false);
    Field::Info             info(Field, bool autoKeyToName=false);

    bool                    is(Class derived, Class base);
    //bool                    is_all(Class, std::initializer_list<Class>);
    //bool                    is_any(Class, std::initializer_list<Class>);

    String                  key(Class);
    String                  key(Field);

    String                  label(Class);
    String                  label(Field);
    
    //Leaf                    leaf(Atom

    ClassData::Shared       merged(Class, unsigned int opts=0);
    Class                   make_class(const String&, const Root* rt=nullptr);

    String                  name(Class);
    String                  name(Field);
    
    NKI                     nki(Class, bool autoKeyToName=false);
    NKI                     nki(Field, bool autoKeyToName=false);
    
    //Vector<Atom>            outbound(Atom);
    ClassVec                outbound(Class, Sorted sorted=Sorted());
    

    Class                   parent(Field);
    String                  pkey(Field);    //!< Key for plurals
    
    String                  plural(Class);
    String                  plural(Field);
    
    QList<QVariant>         qvar_list(const Set<Class>&);
    QList<QVariant>         qvar_list(const Set<Field>&);


    //! \brief Returns the FIRST class fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    ClassFile::Shared       read(Class, const Root*);

    Vector<Class::FragDoc>  reads(Class);
    Vector<Class::FragDoc>  reads(Class, class Root*);

    ClassVec                reverses(Class, Sorted sorted=Sorted());
    
    ClassVec                sources(Class, Sorted sorted=Sorted());
    size_t                  sources_count(Class);
    
    ClassVec                targets(Class, Sorted sorted=Sorted());
    size_t                  targets_count(Class);
    
    TagVec                  tags(Class, Sorted sorted=Sorted());
    TagVec                  tags(Field, Sorted sorted=Sorted());
    size_t                  tags_count(Class);
    size_t                  tags_count(Field);

    bool                    tagged(Class, Tag);
    bool                    tagged(Field, Tag);

    ClassVec                uses(Class, Sorted sorted=Sorted());
    size_t                  uses_count(Class);


    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    ClassFile::Shared       writable(Class, const Root*);


}


