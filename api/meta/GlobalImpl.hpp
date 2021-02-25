////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Global.hpp"
#include "Variant.hpp"

template <typename T>
class Global::Writer {
public:
    Writer& description(const String&s);
    Writer& label(const String&);
    Writer& tag(const String&, const Variant&);
    
    template <bool (*FN)(const T&)>
    Writer& set();
    
    template <void (*FN)(const T&)>
    Writer& set();
    
    template <bool (*FN)(T)>
    Writer& set();

    template <void (*FN)(T)>
    Writer& set();

    Writer(Typed<T>*t) : m_info(t) {}

private:
    Typed<T>*          m_info;
};


template <typename T>
class Global::Typed : public Global {
public:
    typedef T    (*FNGet)();
    typedef bool (*FNSet)(const Variant&);
    
    Typed(const char* k, FNGet fget) :  
        Global(k, metaValue<T>()), m_get(fget), m_set(nullptr)
    {
    }
    
    Variant    value() const override
    {
        return Variant(m_get());
    }

    bool        can_set() const override
    {
        return m_set != nullptr;
    }
    
    bool        set(const Variant& v) const override
    {
        auto tmp    = v.value<T>();
        if(!tmp.good)
            return false;
        return m_set(tmp.value);
    }
    

private:
    friend class Writer<T>;

    FNGet   m_get;
    FNSet   m_set;
};

template <typename T>
Global::Writer<T>    global(const char* k, T(*fn)())
{
    return new Global::Typed<T>(k, fn);
}


//  ----------------------------------------------------------------------------------------------------------------


template <typename T>
Global::Writer<T>& Global::Writer<T>::description(const String&s)
{
    m_info -> description(s);
    return *this;
}


template <typename T>
Global::Writer<T>& Global::Writer<T>::label(const String&s)
{
    m_info -> label(s);
    return *this;
}

template <typename T>
Global::Writer<T>& Global::Writer<T>::tag(const String&k, const Variant&v)
{
    m_info -> tag(k,v);
    return *this;
}
    
template <typename T>
    template <bool (*FN)(const T&)>
Global::Writer<T>& Global::Writer<T>::set()
{
    m_info -> m_set = FN;
    return *this;
}
    
template <typename T>
    template <void (*FN)(const T&)>
Global::Writer<T>& Global::Writer<T>::set()
{
    m_info -> m_set = [](const T& v) -> bool {
        FN(v);
        return true;
    };
    return *this;
}
    
template <typename T>
    template <bool (*FN)(T)>
Global::Writer<T>& Global::Writer<T>::set()
{
    m_info -> m_set = [](const T&v) -> bool {
        return FN(v);
    };
    return *this;
}

template <typename T>
    template <void (*FN)(T)>
Global::Writer<T>& Global::Writer<T>::set()
{
    m_info -> m_set = [](const T&v) -> bool {
        FN(v);
        return true;
    };
    return *this;
}
