////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/kernel/atom/Class.hpp>
#include <basic/Map.hpp>
#include <basic/Set.hpp>
#include <memory>

namespace yq {

    struct Class::Data {
        std::string             name;           //!< Name of this (may or may not be a key)
        std::string             plural;         //!< Plural
        std::string             brief;          //!< Brief description
        std::string             notes;          //!< User scoped notes
        std::string             folder;         //!< Standard folder for these?
        string_set_t            use;            //!< Inheritance (we'll need circular dependency checking)
        
        // Following is being nixed (auto-deduced instead?)
        string_set_t            reverse;        //!< Known Reverse elements (won't trigger)
        
        // Following is being nixed (auto-deduced instead?)
        string_set_t            sources;        //!< Source classes (if edge)

        // Following is being nixed (auto-deduced instead?)
        string_set_t            targets;        //!< Target classes (if edge)

        // Need these to be auto-deduced from usage?
        // Following will be expanded upon... (in database)
        string_set_t            prefixes;       //!< List of common prefixes

        // Need these to be auto-deduced from usage?
        // Following will be expanded upon... (in database)
        string_set_t            suffixes;       //!< List of common suffixes

        //  Following will be expanded upon... (in database)
        //  (auto deduced from DTN?)
        string_set_t            aliases;        //!< List of aliases

        // Following will be expanded upon... (in database)
        string_set_t            tags;           //!< List of tags applied to this class

        // Following will be expanded upon... (in database)
        std::string             category;       //!< Category for this class to be under
        std::string             binding;        //!< Primary Column that binds for edges.   Empty will imply the key as default.
        
        std::string             url;            //!< Path for the URL for type
        std::string             dev_url;        //!< Path for the URL for type

        Data&                   merge(const Data&, bool fOverride);
    };
}
