////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PropGetter.hpp"

namespace yq {
    /*! \brief Abstract getter of type on class/struct
    
        This is an abstract getter bound to a specific data type/class.  Derived getters from this class only
        need to implement the "GET" routine and the constructor.

        \tparam T   variable data type
        \tparam C   bound class/struct's meta info
    */
    template <typename C, typename T>
    class DynamicPropGetter : public PropGetter {
    public:

        virtual const Meta&     data() const override
        {
            return meta<T>();
        }

        virtual const Meta&     object() const override
        {
            return meta<C>();
        }

    protected:
        DynamicPropGetter(PropertyInfo* propInfo, const std::source_location& sl) : PropGetter(propInfo, sl) 
        {
        }
    };
    

    /* !\brief PropGetter for a member variable
    
        PropGetter for the scenario of a member variable on an object.
    */
    template <typename C, typename T>
    class IPM_PropGetter : public DynamicPropGetter<C,T> {
    public:
        typedef const T (C::*P);
        IPM_PropGetter(PropertyInfo* propInfo, const std::source_location& sl, P pointer) : DynamicPropGetter<C,T>(propInfo, sl), m_data(pointer) 
        {
            assert(pointer);
        }

        virtual bool            get(void*dst, const void*obj) const override
        {
            assert(dst);
            assert(obj);
            *(T*) dst   = (((C*) obj)->*m_data);
            return true;
        }
        
    private:
        P       m_data;
    };
    
    template <typename C, typename T>
    class IFV_PropGetter : public DynamicPropGetter<C,T> {
    public:
        typedef T (C::*FN)() const;
        IFV_PropGetter(PropertyInfo* propInfo, const std::source_location& sl, FN function) : DynamicPropGetter<C,T>(propInfo, sl), m_function(function)
        {
            assert(function);
        }
        
        virtual bool        get(void* dst, const void* obj) const override
        {
            assert(dst);
            assert(obj);
            *(T*) dst   = (((const C*) obj)->*m_function)();
            return true;
        }
        
    private:
        FN      m_function;
    };
    
    
    template <typename C, typename T>
    class IFR_PropGetter : public DynamicPropGetter<C,T> {
    public:
        typedef const T& (C::*FN)() const;
        IFR_PropGetter(PropertyInfo* propInfo, const std::source_location& sl, FN function) : DynamicPropGetter<C,T>(propInfo, sl), m_function(function)
        {
            assert(function);
        }
        
        virtual bool        get(void* dst, const void* obj) const override
        {
            assert(dst);
            assert(obj);
            *(T*) dst   = (((const C*) obj)->*m_function)();
            return true;
        }
        
    private:
        FN      m_function;
    };

}
