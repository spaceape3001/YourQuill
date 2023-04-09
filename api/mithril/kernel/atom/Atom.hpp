////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/preamble.hpp>

namespace yq {
    /*! \brief Atom in the cache
        This structure represents an atom in the cache
    */
    struct Atom {
        struct Info;
        using Lock = IDLock<Atom>;
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Atom&rhs) const noexcept = default; 
        constexpr operator uint64_t() const noexcept { return id; }
        
        //  --------------------------------------
        //  These all QUERY the cache (either direct or via CURL)
        //  --------------------------------------
        
        /*! \brief Returns all atoms in the cache database
            \param[in] sorted   Yes/no for sorting by key. (default is no)
            \return std::vector of the found atoms
        */
        static std::vector<Atom>   all(Sorted sorted=Sorted{});

        /*! \brief Returns all atoms in the cache database for given class
        
            This will select ALL atoms that have listed (directly or indirectly) 
            as using the specified atom class.
        
            \param[in] cls      Class to select for
            \param[in] sorted   Yes/no for sorting by key (default is no)
            \return std::vector of the found atoms
        */
        static std::vector<Atom>   all(Class cls, Sorted sorted=Sorted{});

        /*! \brief Returns all atoms in the cache database in the given document
        
            This will select ALL atoms that have been defined by the specified 
            document
        
            \param[in] doc      Document to select for
            \param[in] sorted   Yes/no for sorting by key (default is no)
            \return std::vector of the found atoms
        */
        static std::vector<Atom>   all(Document doc, Sorted sorted=Sorted{});

        /*! \brief Returns all atoms in the cache database with the given tag
        
            This will select ALL atoms that have been tagged by the specified
            tag.
        
            \param[in] tag      Tag to select for
            \param[in] sorted   Yes/no for sorting by key (default is no)
            \return std::vector of the found atoms
        */
        static std::vector<Atom>   all(Tag tag, Sorted sorted=Sorted{});
        
        /*! \brief Counts the number of atoms in the database
            \return The count
        */
        static size_t   count();

        /*! \brief Counts the number of atoms in the database for the given class
        
            This will count ALL atoms that have listed (directly or indirectly)
            as using the specified atom class.
            
            \param[in] cls      Class to select for
            \return The count
        */
        static size_t   count(Class cls);

        /*! \brief Counts the number of atoms in the database tagged by the given document
        
            This counts all atoms in the database that come from the specified document.
            
            \param[in] doc      Document to select for
            \return The count
        */
        static size_t   count(Document doc);

        /*! \brief Counts the number of atoms in the database tagged by the given tag
        
            This counts all atoms in the database that have been tagged by the specified
            tag.
            
            \param[in] tag      Tag to select for
            \return The count
        */
        static size_t   count(Tag tag);
        
        //static Atom     create(Document);

        static bool     exists(uint64_t);
        
        //! Gets atom by ID
        //! \note This verifies the atom's in the database, returns that or NULL
        static Atom     find(uint64_t);
        
        //! Gets atom by FULL key
        static Atom     find(std::string_view);

        /*! Gets root atom of Document
            \param[in] doc  Document of interest
        */
        static Atom     find(Document doc);

        /*! Gets atom by document and sub-key
            \param[in] doc  Document of interest
            \param[in] ck   Sub-key for the atom
        */
        static Atom     find(Document doc, std::string_view ck);
        
        /*! Computes a full key based on document and sub-key
            \param[in] doc  Document of interest
            \param[in] ck   Sub-key for the atom
        */
        static std::string  full_key(Document doc, std::string_view ck);
        
        /*! Gets all atoms with the specified name

            \param[in] n        The name
            \param[in] sorted   Yes/no for sorting by key (default is no)
        */
        static std::vector<Atom>    named(std::string_view n, Sorted sorted=Sorted{});

        /*! \brief Returns the short abbreviation for atom
            \return The standard abbreviation
        */
        std::string         abbreviation() const;

        //! Brief description of atom
        std::string         brief() const;
        
        /*! \brief Children of atom
            \param[in] sorted   Yes/no for sorting by key (default is no)
        */
        std::vector<Atom>   children(Sorted sorted=Sorted{}) const;
        //! Count of child atoms
        size_t              children_count() const;
        
        /*! \brief Classes of the atom
            \param[in] sorted   Yes/no for sorting by key (default is no)
        */
        std::vector<Class>  classes(Sorted sorted=Sorted{}) const;
        
        //! Count of atom's classes
        size_t              classes_count() const;
        
        //! Atom's owning document
        Document            document() const;
        
        //! Atom's icon (from database)
        Image               icon() const;
        
        //! Atom's general information (ie the atom-table lookup in one call)
        Info                info() const;
        
        //! TRUE if the atom is of class
        bool                is(Class) const;
        
        //! TRUE if the atom is of tag
        bool                is(Tag) const;
        
        //! TRUE if the atom is an edge (ie any class lists it as an edge)
        bool                is_edge() const;
        
        //! TRUE if the atom is a leaf
        bool                is_leaf() const;

        //! Key of this atom
        std::string         key() const;
        
        //! A composite, using the name if specified, otherwise, choosing of the keys
        std::string         label() const;

        //! Leaf *IF* this is the document atom and that was a leaf
        Leaf                leaf() const;
        
        /*! Name of the atom
        */
        std::string         name() const;
        
        /*! Name-Key-Icon of atom
            
            For faster lookups of these three items. 
            
            \param[in] autoKeyToName    Will substitute in the key for no name
        */
        NKI                 nki(bool autoKeyToName=false) const;
        
        //! Parent/owning atom
        Atom                parent() const;
        
        //! Child-sub-key for this atom
        std::string         skey() const;
        
        /*! \brief Tags on this atom
            \param[in] sorted   Yes/no for sorting by key (default is no)
        */
        std::vector<Tag>    tags(Sorted sorted=Sorted{}) const;
        
        //! Number of tags on this atom
        size_t              tags_count() const;
        
        //! Atom's "TITLE" 
        std::string         title() const;
        
        //! TRUE if id is non-zero AND in the database
        bool                valid() const;
    };

}

YQ_TYPE_DECLARE(yq::Atom)

