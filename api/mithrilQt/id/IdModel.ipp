////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IdModel.hpp"

namespace yq::mithril {

    IdModel::IdModel(IdProvider::UPtr p) : m_provider(std::move(p))
    {
        assert(m_provider);
    }
    
    IdModel::IdModel(IdProvider::UPtr p, std::vector<IdFilter::UPtr>&&filters) : m_provider(std::move(p)), m_filters(std::move(filters))
    {
        assert(m_provider);
    }
    
    IdModel::~IdModel()
    {
    }
    
    std::vector<Id>     IdModel::_fetch() const
    {
        std::vector<Id> ret;
        if(m_provider){
            std::vector<Id> them    = m_provider->fetch();
            if(m_filters.empty()){
                ret = std::move(them);
            } else {
                ret.reserve(them.size());
                for(Id i : them){
                    bool    rej = false;
                    for(auto& f : m_filters){
                        if(!f->accept(i)){
                            rej = true;
                            break;
                        }
                    }
                    if(!rej)
                        ret.push_back(i);
                }
            }
            
        }
        return ret;
    }

    void            IdModel::setFilters(std::vector<IdFilter::UPtr>&& filters)
    {
        m_filters   = std::move(filters);
    }
}
