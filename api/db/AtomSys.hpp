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



/*! \brief Atom in the cache
    This structure represents an atom in the cache
*/
struct Atom {
    struct Info;
    uint64_t  id  = 0ULL;
    constexpr auto    operator<=>(const Atom&rhs) const = default; //  = default;
    constexpr operator bool() const { return id != 0ULL; }
};


/*! \brief Class in the cache

    This structure represents an atom classification in the cache
*/
struct Class {
    static constexpr const char*    szExtension = "cls";
    struct Info;
    using FragDoc       = std::pair<Fragment, ClassFile::Shared>;

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


struct Atom::Info {
    QString     abbr;
    QString     brief;
    QString     key;
    Document    doc;
    //Leaf        leaf; // causes circular dependencies.... 
    Image       icon;
    auto operator<=>(const Info&) const = default;
};

struct Class::Info {
    QString     key;
    Graph       deps;
    Document    doc;
    Image       icon;
    QString     name;
    QString     plural;
    QString     brief;
    bool        edge;

    Info() : edge{} {}
    auto operator<=>(const Info&) const = default;
};

struct Field::Info {
    Class       class_;
    QString     brief;
    QString     key, pkey;
    QString     name, plural;
    auto operator<=>(const Info&) const = default;
};

namespace cdb {
    using ClassPair     = std::pair<Class,Class>;
    
    /*! \brief Returns the short abbreviation for atom
    
        \param[in]  a   Atom of interest
        \return The standard abbreviation
    */
    QString                 abbreviation(Atom a);

    QStringSet              aliases(Field);

    /*! \brief Returns all atoms in the cache database
        \param[in] sorted   Yes/no for sorting by key. (default is no)
        \return Vector of the found atoms
    */
    Vector<Atom>            all_atoms(Sorted sorted=Sorted());
    
    /*! \brief Returns all atoms in the cache database for given class
    
        This will select ALL atoms that have listed (directly or indirectly) 
        as using the specified atom class.
    
        \param[in] cls      Class to select for
        \param[in] sorted   Yes/no for sorting by key (default is no)
        \return Vector of the found atoms
    */
    Vector<Atom>            all_atoms(Class cls,Sorted sorted=Sorted());
    
    /*! \brief Returns all atoms in the cache database with the given tag
    
        This will select ALL atoms that have been tagged by the specified
        tag.
    
        \param[in] tag      Tag to select for
        \param[in] sorted   Yes/no for sorting by key (default is no)
        \return Vector of the found atoms
    */
    Vector<Atom>            all_atoms(Tag tag,Sorted sorted=Sorted());
    
    /*! \brief Counts the number of atoms in the database
        \return The count
    */
    size_t                  all_atoms_count();
    
    /*! \brief Counts the number of atoms in the database for the given class
    
        This will count ALL atoms that have listed (directly or indirectly)
        as using the specified atom class.
        
        \param[in] cls      Class to select for
        \return The count
    */
    size_t                  all_atoms_count(Class cls);
    
    /*! \brief Counts the number of atoms in the database tagged by the given tag
    
        This counts all atoms in the database that have been tagged by the specified
        tag.
        
        \param[in] tag      Tag to select for
        \return The count
    */
    size_t                  all_atoms_count(Tag tag);

    /*! \brief All classes in the cache database
    
        \param[in]  sorted  Yes/No to sort by key (default is no)
        \return vector of classes
    */
    Vector<Class>           all_classes(Sorted sorted=Sorted());
    
    /*! \brief Count of all classes declared in the cache
    */
    size_t                  all_classes_count();


    Vector<Field>           all_fields(Sorted sorted=Sorted());
    size_t                  all_fields_count();


    Atom                    atom(uint64_t);
    Atom                    atom(const QString&);
    
    Vector<Atom>            atoms(Document, Sorted sorted=Sorted());
    size_t                  atoms_count(Document);
    
    QString                 brief(Atom);
    QString                 brief(Class);
    QString                 brief(Field);

    Class                   class_(Field);
    Class                   class_(uint64_t);
    Class                   class_(const QString&);

    ClassFile::Shared       class_doc(Fragment, bool fAllowEmpty=false);
    
    Vector<Class>           classes(Atom, Sorted sorted=Sorted());
    Vector<Class>           classes(Field);
    Vector<Class>           classes(const StringSet&,Sorted sorted=Sorted());
    Vector<Class>           classes(const QStringSet&,Sorted sorted=Sorted());
    size_t                  classes_count(Atom);
    
    
    QStringSet              data_types(Field);
    
    /*! \brief Creates an atom in the CACHE database
    
        This method creates or returns an atom in the cache database, assuming 
        the document is the atom.
        
        \param[in]      doc         Document to create the atom by
        \param[out]     wasCreated  true/false if the atom was created by this call
        \return Valid Atom if successful.  
        
        \note This should only fail if the document is invalid (ie, no key), or the cache database is corrupted.
    */
    Atom                    db_atom(Document doc, bool* wasCreated=nullptr);
    
    /*! \brief Creates an atom in the CACHE database
    
        This method creates or returns an atom in the cache database
        
        
        \param[in]      doc         Document to create the atom within
        \param[in]      k           Child key (leave empty for document-scoped atom)
        \param[out]     wasCreated  true/false if the atom was created by this call
        \note This should only fail if the document is invalid (ie, no key), or the cache database is corrupted.
    */
    Atom                    db_atom(Document doc, const QString&k, bool* wasCreated=nullptr);
    
    Class                   db_class(const QString&, bool *wasCreated=nullptr);
    Class                   db_class(Document, bool *wasCreated=nullptr);
    Vector<Class>           db_classes(const QStringSet&);
    Vector<Class>           db_classes(const StringSet&);
    
    Field                   db_field(Class c, const QString&k, bool *wasCreated=nullptr);
    //Field                   db_field(const QString&, bool *wasCreated=nullptr);
    //Field                   db_field(Document, bool *wasCreated=nullptr);
    
    Vector<Class>           def_derived(Class);
    Vector<Field>           def_fields(Class);
    Vector<Class>           def_reverse(Class);
    Vector<Class>           def_source(Class);
    Vector<Class>           def_target(Class);
    Vector<Class>           def_use(Class);
    
    Graph                   dep_graph(Class);
    Folder                  detail_folder(Class);

    Vector<Class>           dependents(Class, Sorted sorted=Sorted{});
    
    Document                document(Atom);
    Document                document(Class);
    Document                document(Field);

    bool                    edge(Class);
    
    Vector<Class>           edges_in(Class);
    Vector<Class>           edges_out(Class);
  
    bool                    exists(Atom);
    bool                    exists(Class);
    bool                    exists(Field);

    bool                    exists_atom(uint64_t);
    bool                    exists_class(uint64_t);
    bool                    exists_field(uint64_t);

    Field                   field(uint64_t);
    Field                   field(Class, const QString&);
    Vector<Field>           fields(Class, Sorted sorted=Sorted());
    size_t                  fields_count(Class);

    Image                   icon(Atom);
    Image                   icon(Class);
    Image                   icon(Field);

    Atom::Info              info(Atom);
    Class::Info             info(Class, bool autoKeyToName=false);
    Field::Info             info(Field, bool autoKeyToName=false);

    bool                    is(Atom, Class);
    bool                    is(Atom, std::initializer_list<Class>);
    
    bool                    is_leaf(Atom);

    QString                 key(Atom);
    QString                 key(Class);
    QString                 key(Field);

    QString                 label(Atom);
    QString                 label(Class);
    QString                 label(Field);
    
    //Leaf                    leaf(Atom

    ClassData::Shared       merged(Class, unsigned int opts=0);
    Class                   make_class(const QString&, const Root* rt=nullptr);

    QString                 name(Class);
    QString                 name(Field);
    
    /*! \brief Name/Key/Icon for an Atom
    
        \param a                Atom
        \param autoKeyToName    if TRUE, will auto-fill empty names with keys
    */
    NKI                     nki(Atom a, bool autoKeyToName=false);
    NKI                     nki(Class, bool autoKeyToName=false);
    NKI                     nki(Field, bool autoKeyToName=false);
    

    Class                   parent(Field);
    QString                 pkey(Field);    //!< Key for plurals
    
    QString                 plural(Class);
    QString                 plural(Field);
    
    QList<QVariant>         qvar_list(const Set<Atom>&);
    QList<QVariant>         qvar_list(const Set<Class>&);
    QList<QVariant>         qvar_list(const Set<Field>&);


    //! \brief Returns the FIRST class fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    ClassFile::Shared       read(Class, const Root*);

    Vector<Class::FragDoc>  reads(Class);
    Vector<Class::FragDoc>  reads(Class, class Root*);

    Vector<Class>           reverses(Class, Sorted sorted=Sorted{});
    

    
    Vector<Class>           sources(Class, Sorted sorted=Sorted{});
    size_t                  sources_count(Class);
    
    Vector<Class>           targets(Class, Sorted sorted=Sorted{});
    size_t                  targets_count(Class);
    
    Vector<Tag>             tags(Atom,Sorted sorted=Sorted());
    Vector<Tag>             tags(Class,Sorted sorted=Sorted());
    Vector<Tag>             tags(Field,Sorted sorted=Sorted());
    size_t                  tags_count(Atom);
    size_t                  tags_count(Class);
    size_t                  tags_count(Field);

    bool                    tagged(Atom, Tag);
    bool                    tagged(Atom, std::initializer_list<Tag>);


    QString                 title(Atom);

    Vector<Class>           uses(Class, Sorted sorted=Sorted{});
    size_t                  uses_count(Class);


    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    ClassFile::Shared       write(Class, const Root*);


}


