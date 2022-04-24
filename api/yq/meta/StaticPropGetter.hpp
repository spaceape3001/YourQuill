////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PropGetter.hpp"
#include "TypeInfo.hpp"

namespace yq {

    /*! \brief Abstract static/global getter of type
    
        This is an abstract getter bound to a specific data type.  Derived getters from this class only
        need to implement the "GET" routine and the constructor.
        
        \tparam T   data type being bound to
    */
    template <typename T>
    class StaticPropGetter : public PropGetter {
    public:
    
        virtual const Meta&     data() const override
        {
            return meta<T>();
        }

        virtual const Meta&     object() const override
        {
            return invalid();
        }

    protected:
        StaticPropGetter(PropertyInfo* propInfo, const std::source_location& sl) : PropGetter(propInfo, sl) 
        {
        }
    };
    
    template <typename T>
    class XPV_PropGetter : public StaticPropGetter<T> {
    public:
        typedef const T* P;
        
        XPV_PropGetter(PropertyInfo*propInfo, const std::source_location& sl, P pointer) : StaticPropGetter<T>(propInfo, sl), m_data(pointer) 
        {
            assert(pointer);
        }
    
        virtual bool            get(void*dst, const void*) const override
        {
            assert(dst);
            *(T*) dst   = *m_data;
            return true;
        }
        
    private:
        P      m_data;
    };
    
    template <typename T>
    class XFV_PropGetter : public StaticPropGetter<T> {
    public:
        typedef T (*FN)();
        XFV_PropGetter(PropertyInfo* propInfo, const std::source_location& sl, FN function) : StaticPropGetter<T>(propInfo, sl), m_function(function) 
        {
            assert(function);
        }
        
        virtual bool    get(void* dst, const void*) const  override
        {
            assert(dst);
            *(T*) dst   = std::move(m_function());
            return true;
        }

    private:
        FN      m_function;
    };
    
    template <typename T>
    class XFR_PropGetter : public StaticPropGetter<T> {
    public:
        typedef T& (*FN)();
        XFR_PropGetter(PropertyInfo* propInfo, const std::source_location& sl, FN function) : StaticPropGetter<T>(propInfo, sl), m_function(function) 
        {
            assert(function);
        }
        
        virtual bool    get(void* dst, const void*) const  override
        {
            assert(dst);
            *(T*) dst   = std::move(m_function());
            return true;
        }

    private:
        FN      m_function;
    };

    template <typename T>
    class XFCR_PropGetter : public StaticPropGetter<T> {
    public:
        typedef const T& (*FN)();
        XFCR_PropGetter(PropertyInfo* propInfo, const std::source_location& sl, FN function) : StaticPropGetter<T>(propInfo, sl), m_function(function) 
        {
            assert(function);
        }
        
        virtual bool    get(void* dst, const void*) const  override
        {
            assert(dst);
            *(T*) dst   = std::move(m_function());
            return true;
        }

    private:
        FN      m_function;
    };

    template <typename T>
    class XFVR_PropGetter : public StaticPropGetter<T> {
    public:
        typedef void (*FN)(const T&);
        XFVR_PropGetter(PropertyInfo* propInfo, const std::source_location& sl, FN function) : StaticPropGetter<T>(propInfo, sl), m_function(function)
        {
            assert(function);
        }
        
        virtual bool    get(void* dst, const void*) const override
        {
            assert(dst);
            m_function(*(T*) dst);
            return true;
        }

    private:
        FN      m_function;
    };
    
    template <typename T>
    class XFBR_PropGetter : public StaticPropGetter<T> {
    public:
        typedef bool (*FN)(const T&);
        XFBR_PropGetter(PropertyInfo* propInfo, const std::source_location& sl, FN function) : StaticPropGetter<T>(propInfo, sl), m_function(function)
        {
            assert(function);
        }
        
        virtual bool    get(void* dst, const void*) const override
        {
            assert(dst);
            return m_function(*(T*) dst);
        }
    private:
        FN      m_function;
    };
    
}
