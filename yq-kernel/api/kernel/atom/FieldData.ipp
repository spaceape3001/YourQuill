////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldData.hpp"
#include <basic/TextUtils.hpp>

namespace yq {

    Field::Data&  Field::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(pkey, b.pkey, fOverride);
        set_if_empty(name, b.name, fOverride);
        set_if_empty(plural, b.plural, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(category, b.category, fOverride);
        aliases += b.aliases;
        //type.set_if(b.type, fOverride);
        types += b.types;
        //atom.set_if(b.atom, fOverride);
        atoms += b.atoms;
        tags += b.tags;
        set_if_empty(expected, b.expected, fOverride);
        if((fOverride?b.multiplicity:multiplicity) != Multiplicity())
            multiplicity    = b.multiplicity;
        if((fOverride?b.restriction:restriction) != Restriction())
            restriction     = b.restriction;
        if(fOverride?b.max_count:!max_count)
            max_count       = b.max_count;
        //triggers += b.triggers;
        //for(const auto &i : b.values){
            //auto j = values.find(i.first);
            //if(j != values.end())
                //j->second.merge(i.second, fOverride);
            //else
                //values[i.first] = i.second;
        //}
        return *this;
    }

}
