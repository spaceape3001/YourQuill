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

template <typename> class QList;
class QVariant;

namespace yq {

    /*! \brief Atom in the cache
        This structure represents an atom in the cache
    */
    struct Atom {
        struct Info;
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Atom&rhs) const = default; 
        constexpr operator bool() const { return id != 0ULL; }
    };

    struct Atom::Info {
        String      abbr;
        String      brief;
        String      key;
        //Document    doc;  // it's now "plrual"
        //Leaf        leaf; // causes circular dependencies.... 
        String      name;
        Image       icon;
        bool operator==(const Info&) const = default;
    };

    namespace cdb {
        
        /*! \brief Returns the short abbreviation for atom
        
            \param[in]  a   Atom of interest
            \return The standard abbreviation
        */
        String                 abbreviation(Atom a);

        /*! \brief Returns all atoms in the cache database
            \param[in] sorted   Yes/no for sorting by key. (default is no)
            \return Vector of the found atoms
        */
        AtomVec                all_atoms(Sorted sorted=Sorted{});
        
        /*! \brief Returns all atoms in the cache database for given class
        
            This will select ALL atoms that have listed (directly or indirectly) 
            as using the specified atom class.
        
            \param[in] cls      Class to select for
            \param[in] sorted   Yes/no for sorting by key (default is no)
            \return Vector of the found atoms
        */
        AtomVec                 all_atoms(Class cls,Sorted sorted=Sorted());
        
        /*! \brief Returns all atoms in the cache database with the given tag
        
            This will select ALL atoms that have been tagged by the specified
            tag.
        
            \param[in] tag      Tag to select for
            \param[in] sorted   Yes/no for sorting by key (default is no)
            \return Vector of the found atoms
        */
        AtomVec                 all_atoms(Tag tag,Sorted sorted=Sorted());
        
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
        Atom                    atom(const String&);
        Atom                    atom(Document, const String&);
        Vector<Atom>            atoms(Atom, Sorted sorted=Sorted{});
        Vector<Atom>            atoms(Document, Sorted sorted=Sorted());
        
        AtomVec                 atoms_by_name(const String&, Sorted sorted=Sorted{});
        
        size_t                  atoms_count(Atom);
        size_t                  atoms_count(Document);
        
        String                  brief(Atom);
        
        ClassVec                classes(Atom, Sorted sorted=Sorted{});
        ClassVec                classes(Atom, Document, Sorted sorted=Sorted{});
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
        Atom                    db_atom(Document doc, const String&k, bool* wasCreated=nullptr);
        
        
        //Document                document(Atom);
        
        DocVec                  documents(Atom, Sorted sorted=Sorted{});
        size_t                  documents_count(Atom);

      
        bool                    exists(Atom);

        bool                    exists_atom(uint64_t);


        Image                   icon(Atom);
        
        //Vector<Atom>            inbound(Atom);

        Atom::Info              info(Atom);

        bool                    is(Atom, Class);
        //bool                    is_all(Atom, std::initializer_list<Class>);
        //bool                    is_any(Atom, std::initializer_list<Class>);
        
        bool                    is_leaf(Atom);

        String                  key(Atom);

        String                  label(Atom);
        
        //Leaf                    leaf(Atom

        String                  name(Atom);
        
        /*! \brief Name/Key/Icon for an Atom
        
            \param a                Atom
            \param autoKeyToName    if TRUE, will auto-fill empty names with keys
        */
        NKI                     nki(Atom a, bool autoKeyToName=false);
        
        //Vector<Atom>            outbound(Atom);
        
        Atom                    parent(Atom);

        QList<QVariant>         qvar_list(const Set<Atom>&);

        String                  skey(Atom);

        
        TagVec                  tags(Atom, Sorted sorted=Sorted{});
        TagVec                  tags(Atom, Document, Sorted sorted=Sorted{});
        size_t                  tags_count(Atom);
        size_t                  tags_count(Atom, Document);

        bool                    tagged(Atom, Tag);

        String                  title(Atom);    // kept for comptibility

    }

}

