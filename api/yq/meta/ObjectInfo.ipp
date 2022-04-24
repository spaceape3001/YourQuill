////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ObjectInfo.hpp"
#include <yq/log/Logging.hpp>
#include <yq/type/Object.hpp>

namespace yq {
    const Vector<const ObjectInfo*>&   ObjectInfo::all()
    {
        return repo().objects.all;
    }

    const ObjectInfo*        ObjectInfo::find(std::string_view k)
    {
        return repo().objects.lut.first(k, nullptr);
    }

    const ObjectInfo*        ObjectInfo::find(id_t i)
    {
        const Meta* m   = repo().all.value(i, nullptr);
        return (m && m->is_object()) ? static_cast<const ObjectInfo*>(m) : nullptr;
    }

    ObjectInfo::ObjectInfo(std::string_view zName, const std::source_location& sl) : ObjectInfo(zName, nullptr, sl)
    {
    }
    
    ObjectInfo::ObjectInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl) :
        ObjectInfo(zName, const_cast<ObjectInfo*>(&base), sl)
    {
    }

    ObjectInfo::ObjectInfo(std::string_view zName, ObjectInfo* myBase, const std::source_location& sl) : 
        CompoundInfo(zName, sl), m_base(myBase)
    {
        m_flags |= OBJECT;
        Repo& r    = repo();
        if(r.objects.lut.has(zName))
            yCritical() << "Duplicate object registration: " << zName;
        r.objects << this;
    }

    const Vector<const ObjectInfo*>&    ObjectInfo::bases(bool all) const
    {
        return def(all).bases.all;
    }

    const Vector<const ObjectInfo*>&    ObjectInfo::deriveds(bool all) const
    {
        return def(all).derived.all;
    }
    

    bool    ObjectInfo::is_base(const ObjectInfo*oi) const
    {
        for(const ObjectInfo* b = m_base; b; b = b -> m_base){
            if(oi == b)
                return true;
        }
        return false;
    }
    
    bool    ObjectInfo::is_derived(const ObjectInfo*oi) const
    {
        for(const ObjectInfo* b = oi->m_base; b; b = b -> m_base){
            if(b == this)
                return true;
        }
        return false;
    }
    
    const Vector<const MethodInfo*>&    ObjectInfo::methods(bool all) const
    {
        return def(all).methods.all;
    }
    
    const Vector<const PropertyInfo*>&  ObjectInfo::properties(bool all) const
    {
        return def(all).properties.all;
    }
    
    size_t  ObjectInfo::size() const
    {
        return sizeof(Object);
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
