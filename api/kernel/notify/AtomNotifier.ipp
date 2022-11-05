////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomClassNotifier.hpp"

namespace yq {
    struct AtomClassNotifier::Repo {
        std::vector<const AtomClassNotifier*>    all;
        EnumMap<Change,Vector<const AtomClassNotifier*>> byChange;
    };
    

    AtomClassNotifier::Repo&            AtomClassNotifier::repo()
    {
        static Repo s_repo;
        return s_repo;
    }
        
    const std::vector<const AtomClassNotifier*>& AtomClassNotifier::all()
    {
        return repo().all;
    }
    
    const EnumMap<Change,Vector<const AtomClassNotifier*>>&     AtomClassNotifier::change_map()
    {
        return repo().byChange;
    }

    AtomClassNotifier::AtomClassNotifier(Flag<Change> changeMask, Class cls, const std::source_location& sl)
    {
        m_source        = sl;
        m_change        = changeMask;
        m_class         = cls;
        
        Repo& _r = repo();
        _r.all.push_back(this);
        for(Change c : Change::all_values())
            if(changeMask.is_set(c))
                _r.byChange[c] << this;
    }
    
    AtomClassNotifier::~AtomClassNotifier()
    {
    }

    AtomClassNotifier::Writer&     AtomClassNotifier::Writer::description(std::string_view d)
    {
        if(importer)
            importer -> m_description = d;
        return *this;
    }
}
