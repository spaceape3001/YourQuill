////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/atomsys/class.hpp>
#include <yq/collection/Map.hpp>
#include <yq/collection/Set.hpp>
#include <memory>

namespace yq {

    struct Class::Data {
        std::string             name;           //!< Name of this (may or may not be a key)
        std::string             plural;         //!< Plural
        std::string             brief;          //!< Brief description
        std::string             notes;          //!< User scoped notes
        std::string             folder;         //!< Standard folder for these?
        string_set_t            use;            //!< Inheritance (we'll need circular dependency checking)
        string_set_t            reverse;        //!< Known Reverse elements (won't trigger)
        string_set_t            sources;        //!< Source classes (if edge)
        string_set_t            targets;        //!< Target classes (if edge)
        string_set_t            prefixes;       //!< List of common prefixes
        string_set_t            suffixes;       //!< List of common suffixes
        string_set_t            aliases;        //!< List of aliases
        string_set_t            tags;           //!< List of tags applied to this class
        std::string             category;       //!< Category for this class to be under
        std::string             binding;        //!< Primary Column that binds for edges.   Empty will imply the key as default.

        Data&                   merge(const Data&, bool fOverride);
    };
}
