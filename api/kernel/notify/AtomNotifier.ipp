////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomNotifier.hpp"

namespace yq {
    struct AtomNotifier::Repo {
        std::vector<const AtomNotifier*>    all;
        EnumMap<Change,Vector<const AtomNotifier*>> byChange;
    };
    

    AtomNotifier::Repo&            AtomNotifier::repo()
    {
        static Repo s_repo;
        return s_repo;
    }
        
    const std::vector<const AtomNotifier*>& AtomNotifier::all()
    {
        return repo().all;
    }
    
    const EnumMap<Change,Vector<const AtomNotifier*>>&     AtomNotifier::change_map()
    {
        return repo().byChange;
    }

    AtomNotifier::AtomNotifier(Flag<Change> changeMask, const agw::AtomSpec& spec, const std::source_location& sl)
    {
        m_source        = sl;
        m_change        = changeMask;
        m_spec          = spec;
        
        Repo& _r = repo();
        _r.all.push_back(this);
        for(Change c : Change::all_values())
            if(changeMask.is_set(c))
                _r.byChange[c] << this;
    }
    
    AtomNotifier::~AtomNotifier()
    {
    }

    AtomNotifier::Writer&     AtomNotifier::Writer::description(std::string_view d)
    {
        if(importer)
            importer -> m_description = d;
        return *this;
    }
}
