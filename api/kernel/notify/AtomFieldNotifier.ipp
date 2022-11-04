////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomFieldNotifier.hpp"

namespace yq {
    struct AtomFieldNotifier::Repo {
        std::vector<const AtomFieldNotifier*>    all;
        EnumMap<Change,Vector<const AtomFieldNotifier*>> byChange;
    };
    

    AtomFieldNotifier::Repo&            AtomFieldNotifier::repo()
    {
        static Repo s_repo;
        return s_repo;
    }
        
    const std::vector<const AtomFieldNotifier*>& AtomFieldNotifier::all()
    {
        return repo().all;
    }
    
    const EnumMap<Change,Vector<const AtomFieldNotifier*>>&     AtomFieldNotifier::change_map()
    {
        return repo().byChange;
    }

    AtomFieldNotifier::AtomFieldNotifier(Flag<Change> changeMask, Field fld, const std::source_location& sl)
    {
        m_source        = sl;
        m_change        = changeMask;
        m_field         = fld;
        
        Repo& _r = repo();
        _r.all.push_back(this);
        for(Change c : Change::all_values())
            if(changeMask.is_set(c))
                _r.byChange[c] << this;
    }
    
    AtomFieldNotifier::~AtomFieldNotifier()
    {
    }

    AtomFieldNotifier::Writer&     AtomFieldNotifier::Writer::description(std::string_view d)
    {
        if(importer)
            importer -> m_description = d;
        return *this;
    }
}
