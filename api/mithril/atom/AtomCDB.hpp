////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>
#include <mithril/atom/AtomInfo.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/leaf/Leaf.hpp>
#include <vector>

namespace yq::mithril::cdb {
    /*! \brief Returns all atoms in the cache database
        \param[in] sorted   Yes/no for sorting by key. (default is no)
        \return std::vector of the found atoms
    */
    std::vector<Atom>    all_atoms(Sorted sorted=Sorted{});

    /*! \brief Returns all atoms in the cache database for given class
    
        This will select ALL atoms that have listed (directly or indirectly) 
        as using the specified atom class.
    
        \param[in] cls      Class to select for
        \param[in] sorted   Yes/no for sorting by key (default is no)
        \return std::vector of the found atoms
    */
    std::vector<Atom>    all_atoms(Class cls, Sorted sorted=Sorted{});

    /*! \brief Returns all atoms in the cache database defined by the given document
    
        \param[in] doc      Containing document
        \param[in] sorted   Yes/no for sorting by key (default is no)
        \return std::vector of the found atoms
    */
    std::vector<Atom>    all_atoms(Document doc, Sorted sorted=Sorted{});

    /*! \brief Returns all atoms in the cache database with the given tag
    
        This will select ALL atoms that have been tagged by the specified
        tag.
    
        \param[in] tag      Tag to select for
        \param[in] sorted   Yes/no for sorting by key (default is no)
        \return std::vector of the found atoms
    */
    std::vector<Atom>    all_atoms(Tag tag, Sorted sorted=Sorted{});

    /*! \brief Counts the number of atoms in the database
        \return The count
    */
    size_t               count_atoms();

    /*! \brief Counts the number of atoms in the database for the given class
    
        This will count ALL atoms that have listed (directly or indirectly)
        as using the specified atom class.
        
        \param[in] cls      Class to select for
        \return The count
    */
    size_t               count_atoms(Class cls);

    /*! \brief Counts the number of atoms in the database defined by the specified document
    
        This counts all atoms in the database that have been defined by the given document
        
        \param[in] doc      Containing document
        \return The count
    */
    size_t               count_atoms(Document doc);

    /*! \brief Counts the number of atoms in the database tagged by the given tag
    
        This counts all atoms in the database that have been tagged by the specified
        tag.
        
        \param[in] tag      Tag to select for
        \return The count
    */
    size_t               count_atoms(Tag tag);
    //Atom                 create(Document);
    
    bool                exists(Atom a);
    bool                 exists_atom(uint64_t);

    //! Gets atom by ID
    Atom                 find_atom(uint64_t);

    //! Gets atom by key
    Atom                 find_atom(std::string_view);

    //! Gets first atom defined by document
    Atom                 find_atom(Document doc);

    //! Gets atom by document and sub-key
    Atom                 find_atom(Document doc, std::string_view ck);

    //! All atoms with name
    std::vector<Atom>    named_atoms(std::string_view n, Sorted sorted=Sorted{});

    // ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  To distinguish the ones taking in a Atom as a primary argument

    /*! \brief Returns the short abbreviation for atom
    
        \param[in]  a   Atom of interest
        \return The standard abbreviation
    */
    std::string          abbreviation(Atom a);

    std::vector<Attribute>  attributes(Atom, Sorted sorted=Sorted{});

    //! Brief description of atom
    std::string          brief(Atom a);

    //! Child atoms of specified parent
    std::vector<Atom>    children(Atom a,Sorted sorted=Sorted{});

    //! Counts child atoms of specified parent
    size_t               children_count(Atom a);

    //! Classes of the atom
    std::vector<Class>   classes(Atom a,Sorted sorted=Sorted{});

    //! Count of classes for atom
    size_t               classes_count(Atom a);
    
    //! Defining document for atom
    Document             document(Atom);

    //! Icon for atom
    Image                icon(Atom);

    //! Information for atom
    Atom::Info           info(Atom);

    //! TRUE if atom is specified class
    bool                 is(Atom,Class);
    
    //! TRUE if atom has been tagged with given tag
    bool                 is(Atom,Tag);
    
    //! TRUE if the atom is an edge
    bool                 is_edge(Atom);

    //! TRUE if this atom is also a leaf
    bool                 is_leaf(Atom);

    //! Key for the atom
    std::string          key(Atom);

    //! Label for the atom
    std::string          label(Atom);

    //! Leaf for atom (might not have one)
    Leaf                 leaf(Atom);

    //! Name for the atom
    std::string          name(Atom);

    /*! \brief Name/Key/Icon for an Atom
    
        \param a                Atom
        \param autoKeyToName    if TRUE, will auto-fill empty names with keys
    */
    NKI                 nki(Atom,bool autoKeyToName=false);

    //! parent for atom
    Atom                 parent(Atom);

    std::vector<Atom::Property> properties(Atom, Sorted sorted=Sorted{});
    std::vector<Atom::Property> properties(Atom, Attribute, Sorted sorted=Sorted{});
    std::vector<Atom::Property> properties(Atom, Field, Sorted sorted=Sorted{});

    size_t                  properties_count(Atom);
    size_t                  properties_count(Atom, Attribute);
    size_t                  properties_count(Atom, Field);

    //! Sub-key for atom
    std::string          skey(Atom);

    //! Tags for atom
    std::vector<Tag>     tags(Atom,Sorted sorted=Sorted{});


    //! Count of tags for atom
    size_t               tags_count(Atom);
    
    //! Title for the atom
    std::string          title(Atom);
    
    //! True if the atom exists in database
    bool                 valid(Atom);
    
    // ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  Atom Properties
    
    Atom                    atom(Atom::Property);
    
    Attribute               attribute(Atom::Property);
    
    bool                    exists_atom_property(uint64_t);
    bool                    exists(Atom::Property);
    
    Field                   field(Atom::Property);
    
    Atom::PropertyInfo      info(Atom::Property);
    
    std::string             key(Atom::Property);
    std::string             name(Atom::Property);
    
    Atom                    source(Atom::Property);
    Atom                    target(Atom::Property);
    
    // ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    std::vector<ClassU64Pair>   classes_and_hops(Atom);
    ClassCountMap               classes_and_hops_map(Atom);
    
    
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
    Atom                    db_atom(Document doc, std::string_view k, bool* wasCreated=nullptr);
    
    //! Documents that define this atom
    //std::vector<Document>   documents(Atom, Sorted sorted=Sorted{});
    
    //! Count of documents defining/mentioning atom
    //size_t                  documents_count(Atom);

    
    //! Inbound edges for atom
    std::vector<Atom>            inbound(Atom);

    
    //! Outbound edges for atom
    std::vector<Atom>            outbound(Atom);
    
}
