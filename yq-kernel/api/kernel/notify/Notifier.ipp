////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Notifier.hpp"

namespace yq {
    struct Notifier::Repo {
        std::vector<const Notifier*>    all;
        EnumMap<Change,Vector<const Notifier*>> byChange;
    };
    
    Notifier::Repo&  Notifier::repo()
    {
        static Repo s_repo;
        return s_repo;
    }

    const std::vector<const Notifier*>&  Notifier::all()
    {
        return repo().all;
    }
    
    const EnumMap<Change,Vector<const Notifier*>>&     Notifier::change_map()
    {
        return repo().byChange;
    }
    

    Notifier::Notifier(Flag<Change> changeMask, const FileSpec& _spec, int _order, const std::source_location& sl)
    {
        m_source    = sl;
        m_change    = changeMask;
        m_order     = _order;
        m_spec      = _spec;

        Repo&   _r = repo();
        _r.all.push_back(this);
        for(Change c : Change::all_values())
            if(changeMask.is_set(c))
                _r.byChange[c] << this;
    }
    
    Notifier::~Notifier()
    {
    }

    Notifier::Writer&     Notifier::Writer::description(std::string_view d)
    {
        if(importer)
            importer -> m_description = d;
        return *this;
    }


}
