////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/cdb_common.hpp>
#include "struct.hpp"
#include <db/enum/sorted.hpp>
#include <db/image/struct.hpp>
#include <db/leaf/struct.hpp>
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


        Atom                    atom(uint64_t);
        Atom                    atom(std::string_view );
        Atom                    atom(Document, std::string_view );
        std::vector<Atom>       atoms(Atom, Sorted sorted=Sorted{});
        std::vector<Atom>       atoms(Document, Sorted sorted=Sorted());
        
        std::vector<Atom>       atoms_by_name(std::string_view , Sorted sorted=Sorted{});
        
        size_t                  atoms_count(Atom);
        size_t                  atoms_count(Document);
        
        std::string             brief(Atom);
        
        std::vector<Class>      classes(Atom, Sorted sorted=Sorted{});
        std::vector<Class>      classes(Atom, Document, Sorted sorted=Sorted{});
        size_t                  classes_count(Atom);
        size_t                  classes_count(Atom,Document);
        
        
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
        
        
        std::vector<Document>   documents(Atom, Sorted sorted=Sorted{});
        size_t                  documents_count(Atom);

      
        bool                    exists(Atom);

        bool                    exists_atom(uint64_t);


        Image                   icon(Atom);
        
        std::vector<Atom>            inbound(Atom);

        Atom::Info              info(Atom);

        bool                    is(Atom, Class);
        //bool                    is_all(Atom, std::initializer_list<Class>);
        //bool                    is_any(Atom, std::initializer_list<Class>);
        
        bool                    is_leaf(Atom);

        std::string             key(Atom);

        std::string             label(Atom);
        
        Leaf                    leaf(Atom);

        std::string             name(Atom);
        
        /*! \brief Name/Key/Icon for an Atom
        
            \param a                Atom
            \param autoKeyToName    if TRUE, will auto-fill empty names with keys
        */
        NKI                     nki(Atom a, bool autoKeyToName=false);
        
        std::vector<Atom>            outbound(Atom);
        
        Atom                    parent(Atom);

        std::string             skey(Atom);

        
        std::vector<Tag>        tags(Atom, Sorted sorted=Sorted{});
        std::vector<Tag>        tags(Atom, Document, Sorted sorted=Sorted{});
        size_t                  tags_count(Atom);
        size_t                  tags_count(Atom, Document);

        bool                    tagged(Atom, Tag);

        std::string             title(Atom);    // kept for comptibility

    }

}

