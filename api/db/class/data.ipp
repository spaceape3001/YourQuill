////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "class_data.hpp"

namespace yq {

    ClassData&      ClassData::merge(const ClassData&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(plural, b.plural, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(folder, b.folder, fOverride);
        use += b.use;
        reverse += b.reverse;
        sources += b.sources;
        targets += b.targets;
        set_if_empty(binding, b.binding, fOverride);
        triggers += b.triggers;
        suffixes += b.suffixes;
        prefixes += b.prefixes;
        aliases += b.aliases;
        tags += b.tags;

        for(const auto& i : b.fields){
            auto j = fields.find(i.first);
            if(j != fields.end()){
                j->second.merge(i.second, fOverride);
            } else {
                fields[i.first] = i.second;
            }
        }
        return *this;
    }

    void            ClassData::reset()
    {
        name.clear();
        plural.clear();
        brief.clear();
        notes.clear();
        folder.clear();
        use.clear();
        reverse.clear();
        sources.clear();
        targets.clear();
        prefixes.clear();
        suffixes.clear();
        binding.clear();
        fields.clear();
        triggers.clear();
        aliases.clear();
        tags.clear();
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ClassData::Field& ClassData::Field::merge(const Field& b, bool fOverride)
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
        triggers += b.triggers;
        for(const auto &i : b.values){
            auto j = values.find(i.first);
            if(j != values.end())
                j->second.merge(i.second, fOverride);
            else
                values[i.first] = i.second;
        }
        return *this;
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ClassData::ValueInfo&   ClassData::ValueInfo::merge(const ValueInfo&b, bool fOverride)
    {
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        return *this;
    }
}
