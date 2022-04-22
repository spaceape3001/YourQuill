////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PropertyInfo.hpp"
#include "Global.hpp"
#include "MetaLog.hpp"
#include "ObjectInfo.hpp"
#include "TypeInfo.hpp"

#include <yq/type/Variant.hpp>

namespace yq {

    PropertyInfo::PropertyInfo(std::string_view zName, const std::source_location& sl, const TypeInfo&theType, Meta* parentMeta, options_t opts) : Meta(zName, parentMeta, sl), m_type(theType)
    {
        assert(parentMeta);

        m_flags |= PROPERTY | opts;

        if(GlobalInfo* g = to_global(parentMeta)){
            assert(g == &meta<Global>());
            if(g->m_properties.keys.has(zName))
                metaCritical << "Duplicate property on GLOBAL: " << zName;
            g->m_properties << this;
        }
        
        if(ObjectInfo* obj = to_object(parentMeta)){
            if(obj->m_local.properties.keys.has(zName))
                metaCritical << "Duplicate property on object (" << obj->name() << "): " << zName;
            obj->m_local.properties << this;
        }
        
        if(TypeInfo* type = to_type(parentMeta)){
            if(type -> m_properties.keys.has(zName))
                metaCritical << "Duplicate property on type (" << type->name() << "): " << zName;
            type->m_properties << this;
        }
    }

    Variant     PropertyInfo::get(const void* obj) const
    {
        if(m_getter){
            Variant ret(m_type);
            if(m_getter -> get(ret.raw_ptr(), obj))
                return ret;
        }
        return Variant();
    }
    
    bool        PropertyInfo::set(void* obj, const Variant&var) const
    {
        if(!m_setter)
            return false;
        if(var.type().id() == m_type.id())
            return m_setter -> set(obj, var.raw_ptr());
        Variant v2  = var.convert(m_type);
        if(v2.is_valid())
            return m_setter -> set(obj, v2.raw_ptr());
        return false;
    }

}
