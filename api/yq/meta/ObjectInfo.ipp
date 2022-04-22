////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ObjectInfo.hpp"

namespace yq {
    ObjectInfo::ObjectInfo(std::string_view zName, const std::source_location& sl, ObjectInfo* myBase) : CompoundInfo(zName, sl), m_base(myBase)
    {
        m_flags |= OBJECT;

        Repo& r    = repo();
        if(r.objects.lut.has(zName))
            metaCritical << "Duplicate object registration: " << zName;
        r.objects << this;
    }


    void    ObjectInfo::sweep_impl() 
    {
        CompoundInfo::sweep_impl();
        
        m_all           = {};
        m_local.bases   = {};
        m_local.derived = {};

        if(m_base){
            m_local.bases << m_base;
            m_base -> sweep();
            m_base -> m_local.derived << this;
            
            m_all.bases += m_base -> m_all.bases;
            m_all.methods += m_base -> m_all.methods;
            m_all.properties += m_base -> m_all.properties;
            for(ObjectInfo* b = m_base; b; b = b -> m_base)
                m_all.derived << this;
        }
    }

}
