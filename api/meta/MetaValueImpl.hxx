////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MetaFwd.hpp"
#include "MetaField.hpp"
#include "MetaValue.hpp"
#include "Variant.hpp"
#include "util/DelayInit.hpp"

#include <type_traits>

struct MetaField::Writer {
    Writer(MetaField* mf) : m_field(mf) {}
    Writer&     description(const String&);
    Writer&     label(const String&);
    Writer&     tag(const String&, const Variant&v=Variant());
private:
    MetaField*  m_field;
};

template <typename T, typename V>
class TypedMetaField : public MetaField {
public:
    TypedMetaField(const char*k) : MetaField(k, const_cast<MetaValue&>(metaValue<T>()), metaValue<V>()) {}
};

template <typename T, typename V>
class IM_MetaField : public TypedMetaField<T,V> {
public:
    typedef V (T::*P);

    IM_MetaField(const char* k, P p) : TypedMetaField<T,V>(k), m_p(p) {}

    virtual Variant         get(const void* t) const override
    {
        return Variant( ((const T*)t)->*m_p );
    }
    
    virtual bool            set(void*t, const void*v) const override
    { 
        ((T*)t)->*m_p   = *(const V*) v;
        return true; 
    }
    
private:
    P   m_p;
};


template <typename T, typename V>
class XRR_MetaField : public TypedMetaField<T,V> {
public:
    typedef V     (*FNGet)(const T&);
    typedef void  (*FNSet)(T&, const V&);
  
    XRR_MetaField(const char* k, FNGet g, FNSet s) : TypedMetaField<T,V>(k), m_get(g), m_set(s) {}
    
    virtual Variant get(const void* t) const override
    {
        return m_get( *(const T*) t);
    }
    
    virtual bool    set(void* t, const void* v) const override
    {
        m_set( *(T*) t, *(const V*) v);
        return true;
    }
    
private:
    FNGet       m_get;
    FNSet       m_set;
};

template <typename T, typename V>
class IVR_MetaField : public TypedMetaField<T,V> {
public:
    typedef V       (T::*FNGet)() const;
    typedef void    (T::*FNSet)(const V&);
    
    IVR_MetaField(const char* k, FNGet g, FNSet s) : TypedMetaField<T,V>(k), m_get(g), m_set(s) {}

    virtual Variant get(const void* t) const override
    {
        return (((const T*)t)->*m_get)();
    }
    
    virtual bool    set(void* t, const void* v) const override
    {
        (((T*)t)->*m_set)(*(const V*) v);
        return true;
    }
    
private:
    FNGet       m_get;
    FNSet       m_set;
};



template <typename T>
class BaseMetaValueImpl<T, false> : public MetaValue {
public:

protected:
    BaseMetaValueImpl(const char*z, const char* f, unsigned int i) : MetaValue(z, f, i) {}
};

template <typename T>
class BaseMetaValueImpl<T, true> : public MetaValue {
public:

    template <typename U, U (T::*FN)() const>
    void    member_converts()
    {
        static_assert( MetaValue::Binder<U>::Defined, "Type U must be metatype defined!");
        m_convert[&metaValue<U>()] = [](void* a, const void* b)
        {
            *(U*) a = (((const T*) b)->*FN)();
        };
    }
    
    template <String (T::*FN)() const>
    void    member_print()
    {
        m_print = [](const void*a) -> String 
        {
            return (((const T*) a)->*FN)();
        };
    }
    
    template <typename V>
    MetaField::Writer   field(const char*k, V (T::*ptr))
    {
        return new IM_MetaField<T,V>(k, ptr);
    }
    
    template <typename V>
    MetaField::Writer   field(const char* k, V (*g)(const T&), void (*s)(T&, const V&))
    {
        return new XRR_MetaField(k, g, s);
    }
    
    template <typename V>
    MetaField::Writer   field(const char* k, V (T::*g)() const, void (T::*s)(const V&))
    {
        return new IVR_MetaField(k, g, s);
    }
    
protected:
    BaseMetaValueImpl(const char*z, const char* f, unsigned int i) : MetaValue(z, f, i) {}
};

template <typename T>
class MetaValueImpl : public BaseMetaValueImpl<T, std::is_class_v<T>>, public DelayInit {
    using Base = BaseMetaValueImpl<T, std::is_class_v<T>>;
public:

    template <typename U, U(*FN)(const T&)>
    void    converts()
    {
        static_assert( MetaValue::Binder<U>::Defined, "Type U must be metatype defined!");
        MetaValue::m_convert[&metaValue<U>()] = [](void* a, const void* b)
        {
            *(U*) a = FN(*(const T*) b);
        };
    }

    template <typename U, U(*FN)(T)>
    void    converts()
    {
        static_assert( MetaValue::Binder<U>::Defined, "Type U must be metatype defined!");
        MetaValue::m_convert[&metaValue<U>()] = [](void* a, const void* b)
        {
            *(U*) a = FN(*(const T*) b);
        };
    }

    template <typename U, void (*FN)(U&, const T&)>
    void    converts()
    {
        static_assert( MetaValue::Binder<U>::Defined, "Type U must be metatype defined!");
        MetaValue::m_convert[&metaValue<U>()] = [](void* a, const void* b)
        {
            FN(*(U*) a, *(const T*) b);
        };
    }
    

    template <typename U>
    void    casts()
    {
        static_assert( MetaValue::Binder<U>::Defined, "Type U must be metatype defined!");
        MetaValue::m_convert[&metaValue<U>()] = [](void* a, const void* b)
        {
            *(U*) a = U(*(const T*) b);
        };
    }
    
    template <String(*FN)(const T&)>
    void    print()
    {
        MetaValue::m_print     = [](const void* a) -> String {
            return FN(*(const T*) a);
        };
    }

    template <String(*FN)(T)>
    void    print()
    {
        MetaValue::m_print     = [](const void* a) -> String {
            return FN(*(const T*) a);
        };
    }
    
    template <String (*FN)(T)>
    void    io_format()
    {
        MetaValue::m_ioFormat    = [](const void* a) -> String {
            return FN(*(const T*) a);
        };
    }
    
    template <String (*FN)(const T&)>
    void    io_format()
    {
        MetaValue::m_ioFormat    = [](const void* a) -> String {
            return FN(*(const T*) a);
        };
    }
    
    template <bool (*FN)(T&, const String&)>
    void    io_parse()
    {
        MetaValue::m_ioParse     = [](const String&a, void*b) -> bool {
            return FN(*(T*)b, a);
        };
    }
    
    template <bool (*FN)(const String&, T&)>
    void    io_parse()
    {
        MetaValue::m_ioParse     = [](const String&a, void*b) -> bool {
            return FN(a, *(T*)b);
        };
    }
    
    
    

private:
    friend struct MetaValue::Binder<T>;
    
    MetaValueImpl(const char* f, unsigned int i=MetaType::iBAD) : Base( MetaValue::Binder<T>::szDefName, f, i)
    {
        MetaValue::m_defCopy.ctorCopy( T{} );
        MetaValue::m_copyB         = [](DataBlock& dst, const DataBlock&src){
            dst.reference<T>() = src.reference<T>();
        };
        MetaValue::m_ctorCopyR     = [](DataBlock& dst, const void* src){ 
            dst.ctorCopy(*(const T*) src); 
        };
        MetaValue::m_ctorCopyB     = [](DataBlock& dst, const DataBlock& src){ 
            dst.ctorCopy( src.reference<T>());
        };
        MetaValue::m_ctorMove      = [](DataBlock& dst, DataBlock&& src){
            if constexpr (sizeof(T) <= sizeof(DataBlock))
                dst.ctorMove( std::move(src.reference<T>()));
            else
                dst.Pointer = src.Pointer;
        };
        MetaValue::m_dtor          = [](DataBlock& tgt)
        {
            tgt.dtor<T>();
        };
        MetaValue::m_equal         = [](const DataBlock& a, const DataBlock& b) -> bool 
        {
            return a.reference<T>() == b.reference<T>();
        };
        MetaValue::m_moveB         = [](DataBlock& a, DataBlock&&b) 
        {
            a.reference<T>() = std::move( b.reference<T>());
        };
        
        MetaValue::m_size          = sizeof(T);
        MetaValue::m_small         = MetaValue::m_size <= sizeof(DataBlock);
    }
    
    ~MetaValueImpl() {}

    void        initialize() override 
    {
        subInit();
        MetaValue::insert();
    };

    void        subInit();
    
    static const MetaValue&  s_meta; // purely to force the meta type to initialize
};


#define INTERNAL_MV_IMPLEMENT(type, ...)                                                \
    const MetaValue&         MetaValue::Binder<type>::metaValue()                       \
    {                                                                                   \
        return editValue();                                                             \
    }                                                                                   \
    template <>                                                                         \
    void                    MetaValueImpl<type>::subInit()                              \
    {                                                                                   \
        __VA_ARGS__                                                                     \
    }                                                                                   \
    template<> const MetaValue&         MetaValueImpl<type>::s_meta  = metaValue<type>();

#define MV_IMPLEMENT(name, ...)                                                         \
    MetaValueImpl<name>&     MetaValue::Binder<name>::editValue()                       \
    {                                                                                   \
        static MetaValueImpl<name>  *s_ret = new MetaValueImpl<name>(__FILE__);         \
        return *s_ret;                                                                  \
    }                                                                                   \
    INTERNAL_MV_IMPLEMENT(name, __VA_ARGS__)
    


