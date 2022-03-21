////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/enum/Multiplicity.hpp>
#include <yq/enum/Restriction.hpp>
#include <yq/collection/Map.hpp>
#include <yq/collection/Set.hpp>
#include <memory>

namespace yq {

    struct ClassData {
        struct Field;
        struct ValueInfo;
        struct Trigger;
        
        using ValueMap  = Map<std::string,ValueInfo,IgCase>;
        using FieldMap  = Map<std::string,Field,IgCase>;
        
        using Shared    = std::shared_ptr<ClassData>;

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
        std::string             binding;        //!< Primary Column that binds for edges.   Empty will imply the key as default.
        Vector<Trigger>         triggers;
        FieldMap                fields;

        ClassData&              merge(const ClassData&, bool fOverride);
        void                    reset();
    };

    struct ClassData::ValueInfo {
        std::string             brief;
        std::string             notes;
        
        ValueInfo&              merge(const ValueInfo&, bool fOverride);
    };

    struct ClassData::Trigger {
        std::string             type;
        std::string             name;       // optional 
        std::string             brief;
        std::string             notes;
        string_map_t            args;
    };


    struct ClassData::Field {
        std::string             pkey;           //!< Key if plural (ie, eyes vs eye)
        std::string             name;           //!< Name of this (may or may not be a key)
        std::string             plural;         //!< Plural
        std::string             brief;          //!< Brief description
        std::string             notes;          //!< User scoped notes
        std::string             category;
        
        string_set_t            aliases;        //!< Aliases to this field name
        string_set_t            types;          //!< Permitted data types
        string_set_t            atoms;          //!< Atom types allowed here
        string_set_t            tags;           //!< Lis tof tags applied to this field
        std::string             expected;       //!< Expected type
        
        Multiplicity            multiplicity;
        Restriction             restriction;
        uint64_t                max_count = 0;       //!< Maximum count (zero is unlimited)
        ValueMap                values;
        Vector<Trigger>         triggers;
        
        Field&                  merge(const Field&, bool fOverride);
    };
}
