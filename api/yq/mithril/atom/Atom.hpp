////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/id/Id.hpp>

namespace yq::mithril {

    /*! \brief Atom in the cache
        This structure represents an atom in the cache
    */
    struct Atom {
        static constexpr const IdTypeId ID      = 1;
        static constexpr const IdTypes  PARENTS = {};
    
        struct Info;
        struct Property;
        struct PropertyInfo;
        struct Diff;
        
        using Notify  = Notifier<const Diff&>;
        
        using PropertyCSpan     = std::span<const Property>;
        using PropertySet       = std::set<Property>;
        using PropertyVector    = std::vector<Property>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Atom&rhs) const noexcept = default; 
        constexpr operator uint64_t() const noexcept { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
                
        /*! Computes a full key based on document and sub-key
            \param[in] doc  Document of interest
            \param[in] ck   Sub-key for the atom
        */
        static std::string  full_key(Document doc, std::string_view ck);
        static std::string  full_key(Atom at, std::string_view ck);
        
        //! Full subkey for given atom/child (no '#' is included)
        static std::string  sub_key(Atom at, std::string_view ck);
    };
    
    /*! \brief Property of the atoms
    */
    struct Atom::Property {

        static constexpr const IdTypeId ID      = 2;
        static constexpr const IdTypes  PARENTS = {};

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Property&rhs) const noexcept = default; 
        constexpr operator uint64_t() const noexcept { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Atom)
YQ_TYPE_DECLARE(yq::mithril::AtomSet)
YQ_TYPE_DECLARE(yq::mithril::AtomVector)
YQ_TYPE_DECLARE(yq::mithril::Atom::Property)
YQ_TYPE_DECLARE(yq::mithril::Atom::PropertySet)
YQ_TYPE_DECLARE(yq::mithril::Atom::PropertyVector)

