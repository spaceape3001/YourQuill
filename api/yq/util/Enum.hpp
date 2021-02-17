////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Vector.hpp"
#include "Map.hpp"
#include "String.hpp"


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //      MACROS
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

/*! \brief Declares an enumeration

    This declares an enumeration to use the Enum in this class.  This
    enumeration type provides the benefit of an automatic string/value 
    lookup because your definition gets captured by the macro.  It also 
    automatically encapsulates yoru enumeration values in a name scoped
    fashion, so it becomes YourEnum::YourValue syntax to the rest of the 
    code (ie, Java fashion, which helps avoid those name collisions between
    enumerations).
    
    The restriction is in the enumeration values, because the parser has
    to evaluate them (just once).  While the parser can understand "=number" 
    and "=OtherValue", it does not understand fancier expressions, so no math!
    
    
    \note 1: This macro will *also* create a class (cls)_base to be the 
    definition for the templated class to use.
    
    \note 2: This macro will paste the contents of your enumeration into 
             an enum {} declaration, so C rules for enums apply.  
    
    \param cls      Classname to be used.
    \param expimp   Any DLL import/exprt macro that applies (can be left blank)
    \param ...      The enumeration, C style
*/
#define YQ_ENUM(cls, expimp, ...)                                               \
    class expimp cls##_base {                                                   \
    public:                                                                     \
        enum { isEnum = 1 };                                                    \
        enum enum_t { __VA_ARGS__ };                                            \
        static const char* definition() { return #__VA_ARGS__; }                \
        static const char* name() { return #cls; }                              \
        static const EnumDef*  staticEnumInfo()                                 \
        {                                                                       \
            static const EnumDef*     def = new EnumDef(name(), definition());  \
            return def;                                                         \
        }                                                                       \
        auto operator<=>(const cls##_base&) const = default;                    \
    protected:                                                                  \
        constexpr cls##_base(enum_t v) : m_value(v) {}                          \
        enum_t      m_value;                                                    \
    };                                                                          \
    typedef EnumImpl<cls##_base> cls;

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

class EnumDef; 
template <class> class EnumImpl;

class Enum {
public:

    constexpr Enum() : m_def(nullptr), m_value(-1) {}
        
    int                 value() const { return m_value; }
    String              key() const;
    Enum&               operator=(int value);

    bool                operator==(const Enum&b) const;
    bool                operator!=(const Enum&b) const;

    const   EnumDef*    definition() const { return m_def; }

    Enum(const EnumDef *def, int value);

protected:
    template <class E> friend class EnumImpl;
    friend class EnumDef;

private:
    const EnumDef*  m_def;
    int             m_value;
};

class EnumDef {
public:

    EnumDef(const String& name, const String& def);

    const String&           name() const { return m_name; }

    int                     default_value() const { return m_defValue; }
    int                     minimum_value() const;
    int                     maximum_value() const;

    int_r                   value_of(const String& key) const;

    string_r                key_of(int value) const;
    bool                    has_value(int value) const;
    bool                    has_key(const String&) const;

    const Vector<String>&   all_keys() const { return m_keys; }
    Vector<int>             all_values() const;

    Vector<int>             parse_comma_list(const String&)  const;
    String                  make_comma_list(const Vector<int>&) const;

    Enum                    make_enum(int) const;

protected:

private:
    static Vector<std::pair<String,String>>  parse_definition(const String& def);
    
    using Name2Val          = Map<String,int,IgCase>;
    using Val2Name          = Map<int,String>;

    String                 m_name;
    Name2Val               m_name2value;
    Val2Name               m_value2name;
    Vector<String>         m_keys;
    int                    m_defValue;
};

template <class E>
class EnumImpl : public E {
public:
    static_assert(E::isEnum, "E MUST be YQ_ENUM declared!");
    
    static const Vector<String>&        all_keys();
    static Vector<EnumImpl>             all_values();
    static typename E::enum_t           default_value();
    static bool                         has_key(const String& k);
    static bool                         has_value(int);
    static Vector<String>               ordered_keys();
    static Result<typename E::enum_t>   value_for(const String& txt);
    static int                          max_value();
    static int                          min_value();
    
    // To, make this Vector
    static Vector<EnumImpl>         comma_string(const String& k);
    static String                   comma_string(const Vector<EnumImpl>& k);

    EnumImpl() : E(default_value()) {}
    EnumImpl(const E& base) : E(base) {}
    constexpr EnumImpl(typename E::enum_t value) : E(value) {}
    EnumImpl(int value) : E( has_value(value) ? (typename E::enum_t) value : default_value()) {}
    EnumImpl(const String&key, bool *ok=nullptr) : E(value_for(key))
    {
        auto e = value_for(key);
        E::m_value = e.good ? e.value : default_value();
        if(ok)
            *ok  = e.good;
    }
    EnumImpl(const EnumImpl&) = default;
    EnumImpl(EnumImpl&&) = default;

    operator typename E::enum_t () const { return E::m_value; }

    typename E::enum_t  value() const { return E::m_value; }
    
    String     key() const;

    EnumImpl&   operator=(const EnumImpl&b)
    {
        E::m_value = b.E::m_value;
        return *this;
    }
    
    //EnumImpl&   operator=(EnumImpl&&b) 
    //{
        //E::m_value = b.E::m_value;
        //return *this;
    //}

    EnumImpl&   operator=(typename E::enum_t v);

    //auto        operator<=>(const EnumImpl&) const = default;
    //auto        operator<=>(typename E::enum_t v) const { return operator<=>(EnumImpl(v)); }
    //friend auto operator<=>(typename E::enum_t a, const EnumImpl&b){ return EnumImpl(a) <=> b; }

    //bool        operator==(typename E::enum_t rhs) const { return E::m_value == rhs; }
    //bool        operator!=(typename E::enum_t rhs) const { return E::m_value != rhs; }
    
    bool        operator==(const EnumImpl& rhs) const { return E::m_value == rhs.E::m_value; }
    bool        operator!=(const EnumImpl& rhs) const { return E::m_value != rhs.E::m_value; }
    bool        operator<(const EnumImpl& rhs) const { return E::m_value < rhs.E::m_value; }

    friend bool operator==(typename E::enum_t lhs, const EnumImpl& rhs)
    {
       return lhs == rhs.E::m_value;
    }

    friend bool operator!=(typename E::enum_t lhs, const EnumImpl& rhs)
    {
        return lhs != rhs.E::m_value;
    }
    
    EnumImpl    operator++() 
    {
        if(E::m_value < max_value())
            E::m_value = (typename E::enum_t)( 1 + (int) E::m_value);
        return *this;
    }

private:
    static Vector<String> calcOrderedKeys();
    static Vector<EnumImpl> calcAllValues();
    static Vector<EnumImpl>  to_values(const Vector<int>& vals);
    static Vector<int> to_ints(const Vector<EnumImpl>& vals);
};

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //      IMPLEMENTATION
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


inline bool    Enum::operator==(const Enum&b) const
{
    return (m_def == b.m_def) && (m_value == b.m_value);
}

inline bool    Enum::operator!=(const Enum&b) const
{
    return !operator==(b);
}


        //  -------------------------------------------------------------------

template <typename E>
const Vector<String>& EnumImpl<E>::all_keys()
{
    return E::staticEnumInfo()->all_keys();
}

template <typename E>
Vector<EnumImpl<E>>    EnumImpl<E>::all_values()
{
    static Vector<EnumImpl>   result = calcAllValues();
    return result;
}

template <typename E>
Vector<EnumImpl<E>> EnumImpl<E>::calcAllValues()
{
    return to_values(E::staticEnumInfo()->all_values());
}

template <typename E>
Vector<String> EnumImpl<E>::calcOrderedKeys()
{
    Vector<String> keys    = all_keys();
    keys.sort(
        [](const String& a, const String& b) -> bool
        {
            return EnumImpl(a) < EnumImpl(b);
        }
    );
    return keys;
}

template <typename E>
Vector<EnumImpl<E>>     EnumImpl<E>::comma_string(const String& k)
{
    return toValues(E::staticEnumInfo()->parse_comma_list(k));
}

template <typename E>
String          EnumImpl<E>::comma_string(const Vector<EnumImpl>& k)
{
    return E::staticEnumInfo()->make_comma_list( toInts(k));
}

template <typename E>
typename E::enum_t   EnumImpl<E>::default_value()
{
    static const typename E::enum_t   ret = (typename E::enum_t) E::staticEnumInfo() -> default_value();
    return ret;
}

template <typename E>
bool         EnumImpl<E>::has_key(const String& k)
{
    return E::staticEnumInfo()->has_key(k);
}

template <typename E>
bool         EnumImpl<E>::has_value(int v)
{
    return E::staticEnumInfo()->has_value(v);
}

template <typename E>
Vector<String> EnumImpl<E>::ordered_keys()
{
    static Vector<String> result = calcOrderedKeys();
    return result;
}

template <typename E>
Vector<int> EnumImpl<E>::to_ints(const Vector<EnumImpl>& vals)
{
    Vector<int>      ret;
    ret.reserve(vals.size());
    for(EnumImpl v : vals)
        ret << (typename E::enum_t) v;
    return ret;
}

template <typename E>
Vector<EnumImpl<E>>  EnumImpl<E>::to_values(const Vector<int>& vals)
{
    Vector<EnumImpl>     ret;
    ret.reserve(vals.size());
    for(int v : vals){
        if(E::staticEnumInfo()->has_value(v))
            ret << EnumImpl((typename E::enum_t) v);
    }
    return ret;
}

template <typename E>
Result<typename E::enum_t>   EnumImpl<E>::value_for(const String& txt)
{
    int_r   vi  =  E::staticEnumInfo()->value_of(txt);
    return vi.cast_to<typename E::enum_t>();
}

template <typename E>
int     EnumImpl<E>::max_value()
{
    return E::staticEnumInfo() -> maximum_value();
}

template <typename E>
int     EnumImpl<E>::min_value()
{
    return E::staticEnumInfo() -> minimum_value();
}

    //  -------------------------------------------------------------------

template <typename E>
String     EnumImpl<E>::key() const
{
    return E::staticEnumInfo()->key_of(E::m_value);
}

template <typename E>
EnumImpl<E>&   EnumImpl<E>::operator=(typename E::enum_t v)
{
    E::m_value     = v;
    return *this;
}


//template <typename E>
//bool    EnumImpl<E>::operator==(typename E::enum_t rhs) const
//{
    //return E::m_value == rhs;
//}

//template <typename E>
//bool    EnumImpl<E>::operator!=(typename E::enum_t rhs) const
//{
    //return E::m_value != rhs;
//}

