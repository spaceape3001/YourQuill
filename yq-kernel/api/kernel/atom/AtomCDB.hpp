////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/atom/Atom.hpp>
#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Sorted.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/leaf/Leaf.hpp>
#include <vector>

namespace yq {
    struct Class;
    struct Document;
    struct Tag;

    struct Atom::Info {
        std::string     abbr;
        std::string     brief;
        std::string     key;
        Leaf            leaf; 
        std::string     name;
        Image           icon;
        bool operator==(const Info&) const = default;
    };

    namespace cdb {
        
        /*! \brief Returns the short abbreviation for atom
        
            \param[in]  a   Atom of interest
            \return The standard abbreviation
        */
        std::string            abbreviation(Atom a);

        /*! \brief Returns all atoms in the cache database
            \param[in] sorted   Yes/no for sorting by key. (default is no)
            \return std::vector of the found atoms
        */
        std::vector<Atom>       all_atoms(Sorted sorted=Sorted{});
        
        /*! \brief Returns all atoms in the cache database for given class
        
            This will select ALL atoms that have listed (directly or indirectly) 
            as using the specified atom class.
        
            \param[in] cls      Class to select for
            \param[in] sorted   Yes/no for sorting by key (default is no)
            \return std::vector of the found atoms
        */
        std::vector<Atom>       all_atoms(Class cls,Sorted sorted=Sorted());
        
        /*! \brief Returns all atoms in the cache database with the given tag
        
            This will select ALL atoms that have been tagged by the specified
            tag.
        
            \param[in] tag      Tag to select for
            \param[in] sorted   Yes/no for sorting by key (default is no)
            \return std::vector of the found atoms
        */
        std::vector<Atom>       all_atoms(Tag tag,Sorted sorted=Sorted());
        
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

        //! Gets atom by ID
        Atom                    atom(uint64_t);

        //! Gets atom by key
        Atom                    atom(std::string_view );

        //! Gets atom by document and sub-key
        Atom                    atom(Document, std::string_view );
        
        //! Atoms in document
        std::vector<Atom>       atoms(Document, Sorted sorted=Sorted());
        
        //! All atoms with name
        std::vector<Atom>       atoms_by_name(std::string_view , Sorted sorted=Sorted{});
        
        //! Number of atoms defined in document
        size_t                  atoms_count(Document);
        
        //! Brief description of atom
        std::string             brief(Atom);

        //! Child atoms of specified parent
        std::vector<Atom>       child_atoms(Atom, Sorted sorted=Sorted{});

        //! Count of child atoms
        size_t                  child_atoms_count(Atom);

        //! Classes of the atom
        std::vector<Class>      classes(Atom, Sorted sorted=Sorted{});
        
        //std::vector<Class>      classes(Atom, Document, Sorted sorted=Sorted{});
        
        //! Count of classes for atom
        size_t                  classes_count(Atom);
        //size_t                  classes_count(Atom,Document);
        
        
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
        std::vector<Document>   documents(Atom, Sorted sorted=Sorted{});
        
        //! Count of documents defining/mentioning atom
        size_t                  documents_count(Atom);

        //! TRUE if atom exists
        bool                    exists(Atom);

        //! TRUE if atom exists
        bool                    exists_atom(uint64_t);

        //! Icon for atom
        Image                   icon(Atom);
        
        //! Inbound edges for atom
        std::vector<Atom>            inbound(Atom);

        //! Information for atom
        Atom::Info              info(Atom);

        //! TRUE if atom is specified class
        bool                    is(Atom, Class);
        //bool                    is_all(Atom, std::initializer_list<Class>);
        //bool                    is_any(Atom, std::initializer_list<Class>);
        
        //! TRUE if this atom is also a leaf
        bool                    is_leaf(Atom);

        //! Key for the atom
        std::string             key(Atom);

        //! Label for the atom
        std::string             label(Atom);
        
        //! Leaf for atom (might not have one)
        Leaf                    leaf(Atom);
    
        //! Name for the atom
        std::string             name(Atom);
        
        /*! \brief Name/Key/Icon for an Atom
        
            \param a                Atom
            \param autoKeyToName    if TRUE, will auto-fill empty names with keys
        */
        NKI                     nki(Atom a, bool autoKeyToName=false);
        
        //! Outbound edges for atom
        std::vector<Atom>            outbound(Atom);
        
        //! parent for atom
        Atom                    parent(Atom);

        //! Sub-key for atom
        std::string             skey(Atom);

        //! Tags for atom
        std::vector<Tag>        tags(Atom, Sorted sorted=Sorted{});

        //! Tags for atom 
        //std::vector<Tag>        tags(Atom, Document, Sorted sorted=Sorted{});
        
        //! Count of tags for atom
        size_t                  tags_count(Atom);
        //size_t                  tags_count(Atom, Document);

        //! TRUE if the atom has been taged
        bool                    tagged(Atom, Tag);

        //! Title for atom
        std::string             title(Atom);    // kept for comptibility

    }

}

