////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PropertyInfo.hpp"
#include "MetaWriter.hpp"
#include "DynamicPropSetter.hpp"
#include "StaticPropSetter.hpp"

namespace yq {

    template <typename T>
    class PropertyInfo::Writer : public Meta::Writer {
    public:
        Writer&  def_value(const T& val)
        {
            assert(Meta::Writer::m_meta);
            assert(thread_safe_write());
            static_cast<PropertyInfo*>(m_meta) -> m_default.reference<T>() = val;
            return *this;
        }
        
        Writer(PropertyInfo* pi) : Meta::Writer(pi) {}
    };
    
 //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    template <typename T>
    class PropertyInfo::VarW : public Writer<T> {
    public:
    
        Writer<T>&     setter(void(*function)(T))
        {
            assert(function);
            assert(Meta::Writer::m_meta);

            new XFV_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
            return *this;
        }
        
        Writer<T>&     setter(void(*function)(const T&))
        {
            assert(function);
            assert(Meta::Writer::m_meta);
            assert(thread_safe_write());
            new XFR_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
            return *this;
        }
        
        Writer<T>&     setter(bool(*function)(T))
        {
            assert(function);
            assert(Meta::Writer::m_meta);
            assert(thread_safe_write());
            new XFBV_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
            return *this;
        }
        
        Writer<T>&     setter(bool(*function)(const T&))
        {
            assert(function);
            assert(Meta::Writer::m_meta);
            assert(thread_safe_write());
            new XFBR_PropSetter<T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
            return *this;
        }
        
        VarW(PropertyInfo* pi) : Writer<T>(pi) {}
    };

    template <typename C, typename T>
    class PropertyInfo::PropW : public Writer<T> {
    public:
    
        Writer<T>&     setter(void (C::*function)(T))
        {
            assert(function);
            assert(Meta::Writer::m_meta);
            assert(thread_safe_write());
            new IFV_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
            return *this;
        }
        
        Writer<T>&     setter(void (C::*function)(const T&))
        {
            assert(function);
            assert(Meta::Writer::m_meta);
            assert(thread_safe_write());
            new IFR_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
            return *this;
        }
        
        Writer<T>&     setter(bool (C::*function)(T))
        {
            assert(function);
            assert(Meta::Writer::m_meta);
            assert(thread_safe_write());
            new IFBV_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
            return *this;
        }
        
        Writer<T>&     setter(bool (C::*function)(const T&))
        {
            assert(function);
            assert(Meta::Writer::m_meta);
            assert(thread_safe_write());
            new IFBR_PropSetter<C,T>(static_cast<PropertyInfo*>(Meta::Writer::m_meta), function);
            return *this;
        }
    
        PropW(PropertyInfo* pi) : Writer<T>(pi) {}
    };
}
