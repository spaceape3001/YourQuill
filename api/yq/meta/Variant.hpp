////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include "MetaValue.hpp"

/*! \brief Generic Data

    A Variant is generic data, it's meant to be unobtrusive.  
*/
class Variant {
public:

    Variant();
    
    Variant(const Variant&);
    Variant(Variant&&);
    
    Variant(char);
    Variant(char8_t);
    Variant(char32_t);
    Variant(const char*);
    Variant(const char8_t*);
    Variant(const char32_t*);
    Variant(const std::string&);
    Variant(const std::u8string&);
    Variant(const std::u32string&);
    Variant(const std::wstring&);
    Variant(const String&);
#ifdef ENABLE_QT
    Variant(const QString&);
#endif
    
    
    //! Creates a defaulted Variant to the specified meta-type
    Variant(const MetaValue&);
    Variant(const MetaValue*);
    
    template <typename T>
    Variant(T&&);

    ~Variant();

    Variant&        operator=(const Variant&);
    Variant&        operator=(Variant&&);
    
    bool            operator==(const Variant&) const;
    bool            operator!=(const Variant&) const;
    
    //template <typename T>
    //bool        operator==(const T&) const;
    //template <typename T>
    //bool        operator!=(const T&) const;
    
    bool            can_convert_to(const MetaValue&) const;
    template <typename T>
    bool            can_convert_to() const;

    Variant         convert_to(const MetaValue&) const;

    template <typename T>
    Variant         convert_to() const;


    Variant         get_field(const String&) const;
    
    String          io_format() const;
    static Variant  io_parse(const MetaValue&, const String&);
    
    bool            is_valid() const;
    
    
    /*! \brief "Printable" version for debugging/general-output
    
        \note There's no corresponding "parse" (deliberate), see the I/O helpers
    */
    String      print() const;
    
    bool        set_field(const String&, const Variant&);

    const MetaValue& type() const { return *m_type; }

    template <typename T>
    Result<T>   value() const;
    
    Variant(const MetaValue&, const void*);

    const void*         ptr() const 
    {
        return m_type -> small() ? (const void*) m_data.Data : m_data.Pointer;
    };
    
    void*               ptr() 
    {
        return m_type -> small() ? (void*) m_data.Data : m_data.Pointer;
    };

private:
    Variant(MetaValue&&) = delete;   // prohibt temporary metatypes

    const MetaValue*     m_type;
    DataBlock           m_data;
    
    
    template <typename T>
    void    set(T&&val);
};



MV_DECLARE(String)
MV_DECLARE(bool)
MV_DECLARE(double)
MV_DECLARE(float)
MV_DECLARE(int8_t)
MV_DECLARE(int16_t)
MV_DECLARE(int32_t)
MV_DECLARE(int64_t)
MV_DECLARE(uint8_t)
MV_DECLARE(uint16_t)
MV_DECLARE(uint32_t)
MV_DECLARE(uint64_t)

template <typename T>
void Variant::set(T&& val)
{
    using U     = std::decay<T>::type;
    static_assert( MetaValue::Binder<U>::Defined, "Type T must be metatype defined!");
    m_type      = &metaValue<U>();
    if constexpr (std::is_rvalue_reference_v<T>){
        m_data.ctorMove(std::move(val));
    } else {
        m_data.ctorCopy(val);
    }
}

template <typename T>
Variant::Variant(T&& val)
{
    set(val);
}

//template <typename T>
//bool        Variant::operator==(const T&b) const
//{
    //static_assert( MetaValue::Binder<T>::Defined, "Type T must be metatype defined!");
    //if(&metaValue<T>() != m_type)
        //return false;
    //return m_data.reference<T>() == b;
//}

//template <typename T>
//bool        Variant::operator!=(const T&b) const
//{
    //return !b;
//}


//template <typename T>
//bool        operator==(const T& a, const Variant& b)
//{
    //return b.operator==(a);
//}

//template <typename T>
//bool        operator!=(const T& a, const Variant& b)
//{
    //return !b.operator==(a);
//}

template <typename T>
bool        Variant::can_convert_to() const
{
    static_assert( MetaValue::Binder<T>::Defined, "Type T must be metatype defined!");
    return can_convert_to(metaValue<T>());
}

template <typename T>
Variant     Variant::convert_to() const
{
    static_assert( MetaValue::Binder<T>::Defined, "Type T must be metatype defined!");
    return convert_to(metaValue<T>());
}

template <typename T>
Result<T>   Variant::value() const
{
    static_assert( MetaValue::Binder<T>::Defined, "Type T must be metatype defined!");
    if(m_type == &metaValue<T>())
        return m_data.reference<T>();
    auto cvt = m_type -> m_convert.get(&metaValue<T>(), nullptr);
    if(cvt){
        T       tmp;
        (*cvt)(&tmp, ptr());
        return tmp;
    }
    return Result<T>();
}

boolean_r   to_boolean(const Variant&);
double_r    to_double(const Variant&);
float_r     to_float(const Variant&);
int_r       to_int(const Variant&);
int8_r      to_int8(const Variant&);
int16_r     to_int16(const Variant&);
int32_r     to_int32(const Variant&);
int64_r     to_int64(const Variant&);
integer_r   to_integer(const Variant&);
short_r     to_short(const Variant&);
string_r    to_string(const Variant&);
uint8_r     to_uint8(const Variant&);
uint16_r    to_uint16(const Variant&);
uint32_r    to_uint32(const Variant&);
uint64_r    to_uint64(const Variant&);
unsigned_r  to_uinteger(const Variant&);
ushort_r    to_ushort(const Variant&);
