////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/atom/Field.hpp>
#include <kernel/enum/Multiplicity.hpp>
#include <kernel/enum/Restriction.hpp>
#include <basic/Map.hpp>
#include <basic/Set.hpp>
#include <memory>


namespace yq {
    struct Field::Data {
        std::string             pkey;           //!< Key if plural (ie, eyes vs eye)
        std::string             name;           //!< Name of this (may or may not be a key)
        std::string             plural;         //!< Plural
        std::string             brief;          //!< Brief description
        std::string             notes;          //!< User scoped notes
        std::string             category;       //!< Organization of fields
        
        string_set_t            aliases;        //!< Aliases to this field name
        string_set_t            tags;           //!< List tof tags applied to this field
        string_set_t            classes;        //!< Classes *WE* support

        string_set_t            types;          //!< Permitted data types
        string_set_t            atoms;          //!< Atom types allowed here
        std::string             expected;       //!< Expected type/atom

        Multiplicity            multiplicity;
        Restriction             restriction;
        uint64_t                max_count = 0;       //!< Maximum count (zero is unlimited)

        //Vector<Rule>            rules; (going to DTN?)
        
        Data&                  merge(const Data&, bool fOverride);
    };

}
