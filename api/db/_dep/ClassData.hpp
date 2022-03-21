////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/enum/Multiplicity.hpp>
#include <db/enum/Restriction.hpp>
#include <yq/collection/Map.hpp>
#include <yq/collection/Set.hpp>
#include <yq/text/String.hpp>
#include <memory>

namespace yq {

    struct ClassData {
        struct Field;
        struct ValueInfo;
        struct Trigger;
        
        using ValueMap  = Map<String,ValueInfo,IgCase>;
        using FieldMap  = Map<String,Field,IgCase>;
        
        using Shared    = std::shared_ptr<ClassData>;

        String                  name;           //!< Name of this (may or may not be a key)
        String                  plural;         //!< Plural
        String                  brief;          //!< Brief description
        String                  notes;          //!< User scoped notes
        String                  folder;         //!< Standard folder for these?
        StringSet               use;            //!< Inheritance (we'll need circular dependency checking)
        StringSet               reverse;        //!< Known Reverse elements (won't trigger)
        StringSet               sources;        //!< Source classes (if edge)
        StringSet               targets;        //!< Target classes (if edge)
        StringSet               prefixes;       //!< List of common prefixes
        StringSet               suffixes;       //!< List of common suffixes
        StringSet               aliases;        //!< List of aliases
        StringSet               tags;           //!< List of tags applied to this class
        String                  binding;        //!< Primary Column that binds for edges.   Empty will imply the key as default.
        Vector<Trigger>         triggers;
        FieldMap                fields;

        ClassData&              merge(const ClassData&, bool fOverride);
        void                    reset();
    };

    struct ClassData::ValueInfo {
        String                  brief;
        String                  notes;
        
        ValueInfo&              merge(const ValueInfo&, bool fOverride);
    };

    struct ClassData::Trigger {
        String                  type;
        String                  name;       // optional 
        String                  brief;
        String                  notes;
        StringMap               args;
    };


    struct ClassData::Field {
        String                  pkey;           //!< Key if plural (ie, eyes vs eye)
        String                  name;           //!< Name of this (may or may not be a key)
        String                  plural;         //!< Plural
        String                  brief;          //!< Brief description
        String                  notes;          //!< User scoped notes
        String                  category;
        
        StringSet               aliases;        //!< Aliases to this field name
        StringSet               types;          //!< Permitted data types
        StringSet               atoms;          //!< Atom types allowed here
        StringSet               tags;           //!< Lis tof tags applied to this field
        String                  expected;       //!< Expected type
        
        Multiplicity            multiplicity;
        Restriction             restriction;
        uint64_t                max_count = 0;       //!< Maximum count (zero is unlimited)
        ValueMap                values;
        Vector<Trigger>         triggers;
        
        Field&                  merge(const Field&, bool fOverride);
    };
}
