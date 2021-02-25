////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassFile.hpp"
#include "CacheFwd.hpp"
#include "Linkage.hpp"
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

    /*! \brief All DIRECT class dependencies
    
        This returns all direct class dependencies as a form of pairs (first depends on the second/base class).
        \return Vector of pairs
    */
    Vector<ClassPair>       all_class_dependencies();

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
    Vector<Class>           classes(Field, Linkage l=Linkage::Any);
    Vector<Class>           classes(const StringSet&,Sorted sorted=Sorted());
    Vector<Class>           classes(const QStringSet&,Sorted sorted=Sorted());
    size_t                  classes_count(Atom);
    
    
    
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
    
        This method creates or returns an atom in teh cache database
        
        
        \param[in]      doc         Document to create the atom within
        \param[in]      k           Child key (leave empty for document-scoped atom)
        \param[out]     wasCreated  true/false if the atom was created by this call
        \note This should only fail if the document is invalid (ie, no key), or the cache database is corrupted.
    */
    Atom                    db_atom(Document doc, const QString&k, bool* wasCreated=nullptr);
    
    Class                   db_class(const QString&, bool *wasCreated=nullptr);
    Class                   db_class(Document, bool *wasCreated=nullptr);
    Field                   db_field(Class c, const QString&k, bool *wasCreated=nullptr);
    //Field                   db_field(const QString&, bool *wasCreated=nullptr);
    //Field                   db_field(Document, bool *wasCreated=nullptr);
    
    Graph                   dep_graph(Class);

    Vector<Class>           dependents(Class, Linkage=Linkage::Any, Sorted sorted=Sorted());
    Vector<Class>           dependents(Class, Sorted sorted);
    Vector<Class>           dependents(Class, Sorted::Value sorted);
    
    Document                document(Atom);
    Document                document(Class);
    Document                document(Field);

    bool                    edge(Class);
  
    bool                    exists(Atom);
    bool                    exists(Class);
    bool                    exists(Field);

    bool                    exists_atom(uint64_t);
    bool                    exists_class(uint64_t);
    bool                    exists_field(uint64_t);

    Field                   field(uint64_t);
    Field                   field(Class, const QString&);
    Vector<Field>           fields(Class, Linkage l=Linkage::Any, Sorted sorted=Sorted());
    size_t                  fields_count(Class, Linkage l=Linkage::Any);

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

    ClassFile::Shared       merged(Class, unsigned int opts=0);
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


    //! \brief Returns the FIRST class fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    ClassFile::Shared       read(Class, const Root*);

    Vector<Class::FragDoc>  reads(Class);
    Vector<Class::FragDoc>  reads(Class, class Root*);

    Vector<Class>           reverses(Class, Linkage l=Linkage::Any,Sorted sorted=Sorted());
    Vector<Class>           reverses(Class, Sorted sorted);
    Vector<Class>           reverses(Class, Sorted::Value sorted);
    

    
    Vector<Class>           sources(Class, Linkage l=Linkage::Any,Sorted sorted=Sorted());
    Vector<Class>           sources(Class, Sorted sorted);
    Vector<Class>           sources(Class, Sorted::Value sorted);
    size_t                  sources_count(Class, Linkage l=Linkage::Any);
    
    Vector<Class>           targets(Class, Linkage l=Linkage::Any,Sorted sorted=Sorted());
    Vector<Class>           targets(Class, Sorted sorted);
    Vector<Class>           targets(Class, Sorted::Value sorted);
    size_t                  targets_count(Class, Linkage l=Linkage::Any);
    
    Vector<Tag>             tags(Atom,Sorted sorted=Sorted());
    size_t                  tags_count(Atom);

    bool                    tagged(Atom, Tag);
    bool                    tagged(Atom, std::initializer_list<Tag>);


    QString                 title(Atom);

    Vector<Class>           uses(Class, Linkage l=Linkage::Any,Sorted sorted=Sorted());
    Vector<Class>           uses(Class, Sorted sorted);
    Vector<Class>           uses(Class, Sorted::Value sorted);
    size_t                  uses_count(Class, Linkage l=Linkage::Any);


    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    ClassFile::Shared       write(Class, const Root*);


}


