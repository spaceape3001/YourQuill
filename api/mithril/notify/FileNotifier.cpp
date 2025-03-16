////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FileNotifier.hpp"

namespace yq {
    namespace mithril {
        struct FileNotifier::Repo {
            std::vector<const FileNotifier*>    all;
            EnumMap<Change,Vector<const FileNotifier*>> byChange;
        };
        
        FileNotifier::Repo&  FileNotifier::repo()
        {
            static Repo s_repo;
            return s_repo;
        }

        const std::vector<const FileNotifier*>&  FileNotifier::all()
        {
            return repo().all;
        }
        
        const EnumMap<Change,Vector<const FileNotifier*>>&     FileNotifier::change_map()
        {
            return repo().byChange;
        }
        

        FileNotifier::FileNotifier(Flag<Change> changeMask, const FileSpec& _spec, int _order, const std::source_location& sl)
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
        
        FileNotifier::~FileNotifier()
        {
        }

        FileNotifier::Writer&     FileNotifier::Writer::description(std::string_view d)
        {
            if(importer)
                importer -> m_description = d;
            return *this;
        }


    }
}
