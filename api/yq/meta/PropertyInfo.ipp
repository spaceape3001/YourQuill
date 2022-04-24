////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PropertyInfo.hpp"
#include "Global.hpp"
#include "ObjectInfo.hpp"
#include "TypeInfo.hpp"
#include "PropGetter.hpp"
#include "PropSetter.hpp"

#include <yq/log/Logging.hpp>
#include <yq/type/Any.hpp>

namespace yq {

    PropertyInfo::PropertyInfo(std::string_view zName, const std::source_location& sl, const TypeInfo&theType, Meta* parentMeta, options_t opts) : Meta(zName, parentMeta, sl), m_type(theType)
    {
        assert(parentMeta);

        m_flags |= PROPERTY | opts;

        if(GlobalInfo* g = to_global(parentMeta)){
            assert(g == &meta<Global>());
            if(g->m_properties.keys.has(zName))
                yCritical() << "Duplicate property on GLOBAL: " << zName;
            g->m_properties << this;
        }
        
        if(ObjectInfo* obj = to_object(parentMeta)){
            if(obj->m_local.properties.keys.has(zName))
                yCritical() << "Duplicate property on object (" << obj->name() << "): " << zName;
            obj->m_local.properties << this;
        }
        
        if(TypeInfo* type = to_type(parentMeta)){
            if(type -> m_properties.keys.has(zName))
                yCritical() << "Duplicate property on type (" << type->name() << "): " << zName;
            type->m_properties << this;
        }
    }

    Any     PropertyInfo::get(const void* obj) const
    {
        if(m_getter){
            Any ret(m_type);
            if(m_getter -> get(ret.raw_ptr(), obj))
                return ret;
        }
        return Any();
    }

    bool        PropertyInfo::is_state() const
    {
        return static_cast<bool>(flags() & STATE);
    }
    
    bool        PropertyInfo::is_static() const
    {
        return static_cast<bool>(flags() & STATIC);
    }
    
    bool        PropertyInfo::set(void* obj, const Any&var) const
    {
        if(!m_setter)
            return false;
        if(var.type().id() == m_type.id())
            return m_setter -> set(obj, var.raw_ptr());
        Any v2  = var.convert(m_type);
        if(v2.is_valid())
            return m_setter -> set(obj, v2.raw_ptr());
        return false;
    }

}
