////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/preamble.hpp>
#include <kernel/agw/Atom.hpp>
#include <kernel/agw/AtomInfo.hpp>
#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Sorted.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/leaf/Leaf.hpp>
#include <vector>

namespace yq {
    namespace cdb {
        /*! \brief Returns all atoms in the cache database
            \param[in] sorted   Yes/no for sorting by key. (default is no)
            \return std::vector of the found atoms
        */
        agw::AtomVector             all_atoms(Sorted sorted=Sorted{});

        /*! \brief Returns all atoms in the cache database for given class
        
            This will select ALL atoms that have listed (directly or indirectly) 
            as using the specified atom class.
        
            \param[in] cls      Class to select for
            \param[in] sorted   Yes/no for sorting by key (default is no)
            \return std::vector of the found atoms
        */
        agw::AtomVector             all_atoms(agw::Class cls, Sorted sorted=Sorted{});

        /*! \brief Returns all atoms in the cache database defined by the given document
        
            \param[in] doc      Containing document
            \param[in] sorted   Yes/no for sorting by key (default is no)
            \return std::vector of the found atoms
        */
        agw::AtomVector             all_atoms(Document doc, Sorted sorted=Sorted{});

        /*! \brief Returns all atoms in the cache database with the given tag
        
            This will select ALL atoms that have been tagged by the specified
            tag.
        
            \param[in] tag      Tag to select for
            \param[in] sorted   Yes/no for sorting by key (default is no)
            \return std::vector of the found atoms
        */
        agw::AtomVector             all_atoms(Tag tag, Sorted sorted=Sorted{});

        /*! \brief Counts the number of atoms in the database
            \return The count
        */
        size_t                      count_atoms();

        /*! \brief Counts the number of atoms in the database for the given class
        
            This will count ALL atoms that have listed (directly or indirectly)
            as using the specified atom class.
            
            \param[in] cls      Class to select for
            \return The count
        */
        size_t                      count_atoms(agw::Class cls);

        /*! \brief Counts the number of atoms in the database defined by the specified document
        
            This counts all atoms in the database that have been defined by the given document
            
            \param[in] doc      Containing document
            \return The count
        */
        size_t                      count_atoms(Document doc);

        /*! \brief Counts the number of atoms in the database tagged by the given tag
        
            This counts all atoms in the database that have been tagged by the specified
            tag.
            
            \param[in] tag      Tag to select for
            \return The count
        */
        size_t                      count_atoms(Tag tag);
        //agw::Atom                 create(Document);
        bool                        exists_atom(uint64_t);

        //! Gets atom by ID
        agw::Atom                   find_atom(uint64_t);

        //! Gets atom by key
        agw::Atom                   find_atom(std::string_view);

        //! Gets first atom defined by document
        agw::Atom                   find_atom(Document doc);

        //! Gets atom by document and sub-key
        agw::Atom                   find_atom(Document doc, std::string_view ck);

        //! All atoms with name
        agw::AtomVector             named_atoms(std::string_view n, Sorted sorted=Sorted{});

        // ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  To distinguish the ones taking in a agw::Atom as a primary argument

        /*! \brief Returns the short abbreviation for atom
        
            \param[in]  a   agw::Atom of interest
            \return The standard abbreviation
        */
        std::string                 abbreviation(agw::Atom a);

        //! Brief description of atom
        std::string                 brief(agw::Atom a);

        //! Child atoms of specified parent
        agw::AtomVector             children(agw::Atom a,Sorted sorted=Sorted{});

        //! Counts child atoms of specified parent
        size_t                      children_count(agw::Atom a);

        //! Classes of the atom
        agw::ClassVector            classes(agw::Atom a,Sorted sorted=Sorted{});

        //! Count of classes for atom
        size_t                      classes_count(agw::Atom a);
        
        //! Defining document for atom
        Document                    document(agw::Atom);

        //! Icon for atom
        Image                       icon(agw::Atom);

        //! Information for atom
        agw::Atom::Info             info(agw::Atom);


        //! TRUE if atom is specified class
        [[deprecated]] bool         is(agw::Atom,agw::Class);
        
        //! TRUE if atom has been tagged with given tag
        [[deprecated]] bool         is(agw::Atom,Tag);
        

        //! TRUE if the atom is an edge
        bool                        is_edge(agw::Atom);

        //! TRUE if this atom is also a leaf
        bool                        is_leaf(agw::Atom);

        //! TRUE if this atom is of specified class
        bool                        is_of_class(agw::Atom, agw::Class);

        //! TRUE if this atom is tagged
        bool                        is_tagged(agw::Atom, Tag);

        //! Key for the atom
        std::string                 key(agw::Atom);

        //! Label for the atom
        std::string          label(agw::Atom);

        //! Leaf for atom (might not have one)
        Leaf                 leaf(agw::Atom);

        //! Name for the atom
        std::string          name(agw::Atom);

        /*! \brief Name/Key/Icon for an agw::Atom
        
            \param a                agw::Atom
            \param autoKeyToName    if TRUE, will auto-fill empty names with keys
        */
        cdb::NKI             nki(agw::Atom,bool autoKeyToName=false);

        //! parent for atom
        agw::Atom                 parent(agw::Atom);

        //! Sub-key for atom
        std::string          skey(agw::Atom);

        //! Tags for atom
        std::vector<Tag>     tags(agw::Atom,Sorted sorted=Sorted{});

        //! Count of tags for atom
        size_t               tags_count(agw::Atom);
        
        //! Title for the atom
        std::string          title(agw::Atom);
        
        //! True if the atom exists in database
        bool                 valid(agw::Atom);
        
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        std::vector<agw::ClassU64Pair>   classes_and_hops(agw::Atom);
        agw::ClassCountMap               classes_and_hops_map(agw::Atom);
        
        
        /*! \brief Creates an atom in the CACHE database
        
            This method creates or returns an atom in the cache database, assuming 
            the document is the atom.
            
            \param[in]      doc         Document to create the atom by
            \param[out]     wasCreated  true/false if the atom was created by this call
            \return Valid agw::Atom if successful.  
            
            \note This should only fail if the document is invalid (ie, no key), or the cache database is corrupted.
        */
        agw::Atom                    db_atom(Document doc, bool* wasCreated=nullptr);
        
        /*! \brief Creates an atom in the CACHE database
        
            This method creates or returns an atom in the cache database
            
            
            \param[in]      doc         Document to create the atom within
            \param[in]      k           Child key (leave empty for document-scoped atom)
            \param[out]     wasCreated  true/false if the atom was created by this call
            \note This should only fail if the document is invalid (ie, no key), or the cache database is corrupted.
        */
        agw::Atom                    db_atom(Document doc, std::string_view k, bool* wasCreated=nullptr);
        
        //! Documents that define this atom
        //std::vector<Document>   documents(agw::Atom, Sorted sorted=Sorted{});
        
        //! Count of documents defining/mentioning atom
        //size_t                  documents_count(agw::Atom);

        
        //! Inbound edges for atom
        agw::AtomVector            inbound(agw::Atom);

        
        //! Outbound edges for atom
        agw::AtomVector            outbound(agw::Atom);
        
    }

}
