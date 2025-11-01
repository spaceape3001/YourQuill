////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/field/Field.hpp>
#include <yq/mithril/enum/Multiplicity.hpp>
#include <yq/mithril/enum/Restriction.hpp>
#include <yq/container/Map.hpp>
#include <yq/container/Set.hpp>
#include <yq/typedef/string_sets.hpp>
#include <yq/text/IgCase.hpp>
#include <memory>


namespace yq::mithril {
    struct Field::Data {
        std::string             pkey;           //!< Key if plural (ie, eyes vs eye)
        std::string             name;           //!< Name of this (may or may not be a key)
        std::string             plural;         //!< Plural
        std::string             brief;          //!< Brief description
        std::string             notes;          //!< User scoped notes
        std::string             category;       //!< Organization of fields
        
        string_set_t            usurps;         //!< Can usurp these attribute (if otherwise unspecified)
        string_set_t            aliases;        //!< Aliases to this field name
        string_set_t            tags;           //!< List tof tags applied to this field
        string_set_t            classes;        //!< Classes *WE* support

        string_set_t            types;          //!< Permitted data/atom types
        std::string             expected;       //!< Expected type/atom

        Multiplicity            multiplicity;
        Restriction             restriction;
        uint64_t                max_count = 0;       //!< Maximum count (zero is unlimited)

        //Vector<Rule>            rules; (going to DTN?)
        
        Data&                  merge(const Data&, bool fOverride);
    };
}
