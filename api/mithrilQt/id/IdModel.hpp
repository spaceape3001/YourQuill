////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/IdProvider.hpp>
#include <mithril/id/IdFilter.hpp>

namespace yq::mithril {

    class IdModel {
    public:
    
        //! Sets new filters
        //! \note Call a reload after setting filters
        void            setFilters(std::vector<IdFilter::UPtr>&&);
    
    protected:
        IdProvider::UPtr                m_provider;
        std::vector<IdFilter::UPtr>     m_filters;
        std::vector<Id>                 m_data;
        
        IdModel(IdProvider::UPtr);
        IdModel(IdProvider::UPtr, std::vector<IdFilter::UPtr>&&);
        ~IdModel();
        
        std::vector<Id>     _fetch() const;
    };

}

