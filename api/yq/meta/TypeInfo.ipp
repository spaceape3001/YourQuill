////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TypeInfo.hpp"
#include <yq/stream/Ops.hpp>

namespace yq {

    struct EmptyType : public TypeInfo {
    
        EmptyType(id_t i, const std::source_location& sl = std::source_location::current()) : TypeInfo( i ? "Any" : "Void", sl, i) 
        {
            m_copyB     = [](DataBlock&, const DataBlock&) {};
            m_copyR     = [](DataBlock&, const void*) {};
            m_ctorCopyR = [](DataBlock&, const void*) {};
            m_ctorCopyB = [](DataBlock&, const DataBlock&) {};
            m_ctorMove  = [](DataBlock&, DataBlock&&) {};
            m_dtor      = [](DataBlock&) {};
            m_equal     = [](const DataBlock&, const DataBlock&) -> bool { return true; };
            m_moveB     = [](DataBlock&, DataBlock&&) {};
            if(i)
                m_print = [](Stream& s, const void*) { s << "(wildcard)"; };
            else
                m_print = [](Stream& s, const void*) { s << "(empty)"; };
        }
        
        ~EmptyType(){}
    };
        
    const TypeInfo&        invalid()
    {
        static EmptyType*  s_ret = new EmptyType(MT_Invalid);
        return *s_ret;
    }
    
    const TypeInfo&        variant()
    {
        static EmptyType*  s_ret = new EmptyType(MT_Variant);
        return *s_ret;
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    const Vector<const TypeInfo*>&   TypeInfo::all()
    {
        assert(thread_safe_read());
        return repo().types.all;
    }
    
    const TypeInfo*                  TypeInfo::lookup(id_t i)
    {
        assert(thread_safe_read());
        const Meta* m   = repo().all.value(i, nullptr);
        return (m && m->is_type()) ? static_cast<const TypeInfo*>(m) : nullptr;
    }
    
    const TypeInfo*                  TypeInfo::lookup(const std::string_view&sv)
    {
        assert(thread_safe_read());
        return repo().types.lut.first(sv, nullptr);
    }


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    TypeInfo::TypeInfo(std::string_view zName, const std::source_location& sl, id_t i) : CompoundInfo(zName, sl, nullptr, i)
    {
        m_flags |= TYPE;
        
        Repo&   r  = repo();
        assert("no duplicate typenames!" && !r.types.lut.has(zName)); 
        
        r.types << this;
    }

    TypeInfo::~TypeInfo()
    {
        //  should never be used....
    }
    
    void    TypeInfo::sweep_impl() 
    {
        CompoundInfo::sweep_impl();
        gather(m_methods);
        gather(m_properties);
    }
}
