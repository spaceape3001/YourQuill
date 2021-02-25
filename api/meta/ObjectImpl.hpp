////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#define INTERNAL_MO_IMPLEMENT(ObjectClass, ...)                                                         \
    template <> const MetaObject&   TypedMetaObject<ObjectClass>::s_meta = ObjectClass::static_meta();  \
    template <> void TypedMetaObject<ObjectClass>::detail_init()                                        \
    {                                                                                                   \
        __VA_ARGS__                                                                                     \
    }                                                                                                   \
    const ObjectClass::Meta&        ObjectClass::meta() const                                           \
    {                                                                                                   \
        return ObjectClass::static_meta();                                                              \
    } 
        
#define MO_ABSTRACT(ObjectClass, ...)                                                                   \
    INTERNAL_MO_IMPLEMENT(ObjectClass, __VA_ARGS__)                                                     \
    const ObjectClass::Meta&    ObjectClass::static_meta()                                              \
    {                                                                                                   \
        static AbstractMetaObject<ObjectClass>     ret(__FILE__);                                       \
        return ret;                                                                                     \
    }

#define MO_CONCRETE(ObjectClass, ...)                                                                   \
    INTERNAL_MO_IMPLEMENT(ObjectClass, __VA_ARGS__)                                                     \
    const ObjectClass::Meta&    ObjectClass::static_meta()                                              \
    {                                                                                                   \
        static ConcreteMetaObject<ObjectClass>     ret(__FILE__);                                       \
        return ret;                                                                                     \
    }
    
#include "MetaObject.hpp"
#include "MetaProperty.hpp"
#include "MetaValue.hpp"
#include "Object.hpp"
#include "Variant.hpp"

#include "util/DelayInit.hpp"

#include <type_traits>

struct MetaProperty::Writer {
    Writer(MetaProperty* p) : m_prop(p) {}
    
    Writer& label(const String&);
    Writer& description(const String&);
    Writer& tag(const String&, const Variant&);
    
private:
    MetaProperty*   m_prop;
};

template <typename T, typename V>
struct TypedMetaProperty : public MetaProperty {
    TypedMetaProperty(const char* z) : MetaProperty(z, const_cast<MetaObject&>(T::static_meta()), metaValue<V>()) {}
};

template <typename T, typename V>
struct IM_MetaProperty : public TypedMetaProperty<T,V> {
    typedef V (T::*P);
    
    IM_MetaProperty(const char* k, P p) : TypedMetaProperty<T,V>(k), m_p(p) {}
    virtual bool        is_settable() const { return true; }
    
protected:
    virtual Variant     get_impl(const Object&obj) const override
    {
        return (static_cast<const T&>(obj).*m_p);
    }
    
    virtual bool        set_impl(Object&obj, const void*var) const override
    {
        *(static_cast<T&>(obj).*m_p) = *((const V*) var);
        return true;
    }

    P   m_p;
};

template <typename T, typename V>
struct IV_MetaProperty : public TypedMetaProperty<T,V> {
    typedef V (T::*FNGet)() const;
    
    IV_MetaProperty(const char* k, FNGet g) : TypedMetaProperty<T,V>(k), m_get(g) {}
    
protected:
    virtual Variant     get_impl(const Object&obj) const override
    {
        return (static_cast<const T&>(obj).*m_get)();
    }
    
    FNGet       m_get;
};

template <typename T, typename V>
struct IVR_MetaProperty : public TypedMetaProperty<T,V> {
    typedef V (T::*FNGet)() const;
    typedef void (T::*FNSet)(const V&);
    
    IVR_MetaProperty(const char*k, FNGet g, FNSet s) : TypedMetaProperty<T,V>(k), m_get(g), m_set(s) {}

    virtual bool        is_settable() const { return true; }
    
protected:
    virtual Variant     get_impl(const Object&obj) const override
    {
        return (static_cast<const T&>(obj).*m_get)();
    }

    virtual bool        set_impl(Object&obj, const void*var) const override
    {
        (static_cast<T&>(obj).*m_set)(*(const V*) var);
        return true;
    }


    FNGet       m_get;
    FNSet       m_set;
};

template <typename T, typename V>
struct IVV_MetaProperty : public TypedMetaProperty<T,V> {
    typedef V (T::*FNGet)() const;
    typedef void (T::*FNSet)(V);

    IVV_MetaProperty(const char*k, FNGet g, FNSet s) : TypedMetaProperty<T,V>(k), m_get(g), m_set(s) {}

    virtual bool        is_settable() const { return true; }

protected:
    virtual Variant     get_impl(const Object&obj) const override
    {
        return (static_cast<const T&>(obj).*m_get)();
    }

    virtual bool        set_impl(Object&obj, const void*var) const override
    {
        (static_cast<T&>(obj).*m_set)(*(const V*) var);
        return true;
    }

    FNGet       m_get;
    FNSet       m_set;
};

template <typename T>
struct TypedMetaObject : public T::Meta, public DelayInit {

    TypedMetaObject(const char* f) : T::Meta(T::szClassName, f), m_init(false) {}
    
    template <typename U=T>
    std::enable_if_t<std::is_same_v<typename U::Base,void>> base_init()
    {
    }

    template <typename U=T>
    std::enable_if_t<!std::is_same_v<typename U::Base,void>> base_init()
    {
        typename U::Base::Meta* b  = const_cast<typename U::Base::Meta*>(&U::Base::static_meta());
        b->init_me();
        MetaObject::m_base  = b;
    }
    
    void        init_me() override
    {
        if(!m_init){
            base_init();
            static_cast<MetaObject*>(this) -> inherit();
            detail_init();
            static_cast<MetaObject*>(this) -> insert();
            m_init      = true;
        }
    }

    template <typename V>
    MetaProperty::Writer    property(const char*k, V (T::*p))
    {
        return new IM_MetaProperty<T,V>(k,p);
    }
    
    template <typename V>
    MetaProperty::Writer    property(const char*k, V (T::*g)() const)
    {
        return new IV_MetaProperty<T,V>(k,g);
    }

    template <typename V>
    MetaProperty::Writer    property(const char*k, V (T::*g)() const, void (T::*s)(const V&))
    {
        return new IVR_MetaProperty<T,V>(k,g,s);
    }

    template <typename V>
    MetaProperty::Writer    property(const char*k, V (T::*g)() const, void (T::*s)(V))
    {
        return new IVV_MetaProperty<T,V>(k,g,s);
    }

    void        initialize() override 
    {
        init_me();
    }
    
    void        detail_init();
    bool        m_init;
    static const MetaObject&  s_meta;
};

template <typename T> 
struct ConcreteMetaObject : public T::Typed<T> {
    static_assert(!std::is_abstract_v<T>, "Compiler considers T to be abstract (ie, pure virtuals are not overloaded)!");
    ConcreteMetaObject(const char* f) : T::Typed<T>(f){}
    ~ConcreteMetaObject(){}
    virtual T*      allocate() const override { return new T; }
    virtual bool    is_abstract() const override { return false; }
};

template <typename T> 
struct AbstractMetaObject : public T::Typed<T>  {
    AbstractMetaObject(const char* f) : T::Typed<T>(f){}
    ~AbstractMetaObject() {}
    virtual T*      allocate() const override { return nullptr; }
    virtual bool    is_abstract() const override { return true; }
};


