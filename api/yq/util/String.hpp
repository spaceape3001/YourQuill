////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TypeFwd.hpp"
#include <string>
#include <cwchar>
#include <cuchar>

class QByteArray;
class QString;

using u32string_r   = Result<std::u32string>;
using wstring_r     = Result<std::wstring>;

class Char8 {
public:
    constexpr Char8() : m_code(0) {}
    constexpr Char8(char c) : m_code(c) {}
    constexpr Char8(char8_t c) : m_code((char) c) {}
    
    operator char () const { return m_code; }
    char        code() const { return m_code; }
    bool        good() const { return m_code != 0; }
    
    bool        operator==(Char8 b) { return m_code == b.m_code; }
    bool        operator!=(Char8 b) { return m_code != b.m_code; }
    bool        operator<(Char8 b) { return m_code < b.m_code; }
    bool        operator<=(Char8 b) { return m_code <= b.m_code; }
    bool        operator>(Char8 b) { return m_code > b.m_code; }
    bool        operator>=(Char8 b) { return m_code >= b.m_code; }
    
    bool        is_alnum() const;
    bool        is_alpha() const;
    bool        is_blank() const;
    bool        is_control() const;
    bool        is_digit() const;
    bool        is_graph() const;
    bool        is_lower() const;
    bool        is_print() const;
    bool        is_punct() const;
    bool        is_space() const;
    bool        is_upper() const;
    bool        is_xdigit() const;
    
    char        to_lower() const;
    char        to_upper() const;
    
private:
    char        m_code;
};


class Char32 {
public:
    Char32() : m_code(0) {}
    Char32(char32_t c) : m_code(c) {}

    operator char32_t () const { return m_code; }
    
    char32_t    code() const { return m_code; }
    bool        good() const { return m_code != 0; }
    
    //bool        operator==(Char32 b) { return m_code == b.m_code; }
    //bool        operator!=(Char32 b) { return m_code != b.m_code; }
    //bool        operator<(Char32 b) { return m_code < b.m_code; }
    //bool        operator<=(Char32 b) { return m_code <= b.m_code; }
    //bool        operator>(Char32 b) { return m_code > b.m_code; }
    //bool        operator>=(Char32 b) { return m_code >= b.m_code; }

    bool        is_alnum() const;
    bool        is_alpha() const;
    bool        is_blank() const;
    bool        is_control() const;
    bool        is_digit() const;
    bool        is_graph() const;
    bool        is_lower() const;
    bool        is_print() const;
    bool        is_punct() const;
    bool        is_space() const;
    bool        is_upper() const;
    bool        is_xdigit() const;
    
    char32_t    to_lower() const;
    char32_t    to_upper() const;
    
private:
    char32_t    m_code;
};


/*! \brief UTF-8 String

    This is our generic UTF-8 String.
    It won't be perfect on lower/upper/ignore-case due to some languages having multiple letter equivalents.
    
*/
class String : public std::string {
public:

    class Join;
    class Iter32;
    class IterW;

    /*! \brief Creates a boolean string from one or the other
    */
    static String   boolean(bool, const String& kFalse="false", const String& kTrue="true");
    static String   hex(uint8_t, char f='0');
    static String   hex(uint16_t, char f='0');
    static String   hex(uint32_t, char f='0');
    static String   hex(uint64_t, char f='0');

    static String   number(double);
    static String   number(float);

    static String   number(int8_t);
    static String   number(int16_t);
    static String   number(int32_t);
    static String   number(int64_t);

    static String   number(uint8_t);
    static String   number(uint16_t);
    static String   number(uint32_t);
    static String   number(uint64_t);

    static String   scientific(double, int digits=-1);
    static String   scientific(float, int digits=-1);

    using std::string::replace;

    String(){}
    
    String(char ch) : std::string(1,ch) {}
    
    //! Creates single-character string
    String(char8_t ch) : std::string(1,(char) ch) {}
    
    //! Creates single-character string
    String(char32_t);

    String(const char* z) : std::string(z) {}
    String(const char* z, size_t sz) : std::string(z,sz) {}
    String(const char8_t*z) : std::string((const char*) z) {}
    String(const char8_t*z, size_t sz) : std::string((const char*) z, sz) {}
    String(const char32_t*);
    
    
    String(const std::string&cp) : std::string(cp) {}
    String(const std::string_view&cp) : std::string(cp) {}
    
    String(const std::string&cp, size_type pos, size_type count = npos) : std::string(cp, pos, count) {}
    
    String(const std::u8string&);
    String(const std::u8string&, size_type pos, size_type count = npos);
    String(const std::u8string_view&);
    String(const std::u32string&);
    String(const std::u32string_view&);
    String(const std::wstring&);
    String(const std::wstring_view&);
    
    String(size_t n, char ch) : std::string(n, ch) {}
    String(size_t n, char8_t ch) : std::string(n, (char) ch) {}
    String(size_t, char32_t);

    explicit String(const Vector<uint8_t>&);

    String(const String&cp) : std::string(cp) {}
    String(String&&mv) : std::string(std::move(mv)) {}
    String(std::string&&mv) : std::string(std::move(mv)) {}
    
    String(const QString&);
    String(const QByteArray&);
    
    ~String(){}
    
    String&     operator=(const String&cp) 
    {
        std::string::operator=(cp);
        return *this;
    }
    
    String&     operator=(const std::string&cp)
    {
        std::string::operator=(cp);
        return *this;
    }
    
    String&     operator=(const std::u8string&);
    String&     operator=(const std::u32string&);
    String&     operator=(const std::wstring&);
    
    String&     operator=(const char*z) 
    {
        std::string::operator=(z);
        return *this;
    }
    
    String&     operator=(const char8_t*z)
    {
        std::string::operator=((const char*) z);
        return *this;
    }
    
    String&     operator=(const wchar_t*);
    String&     operator=(const char32_t*);
    String&     operator=(const std::string_view&z)
    {
        std::string::operator=(z);
        return *this;
    }
    
    String&     operator=(const std::u8string_view&);
    String&     operator=(const std::u32string_view&);
    String&     operator=(const std::wstring_view&);
    
    String&     operator=(const QByteArray&);
    String&     operator=(const QString&);

    String&     operator=(String&&mv) 
    {
        std::string::operator=(std::move(mv));
        return *this;
    }
    
    String&     operator=(std::string&&mv)
    {
        std::string::operator=(std::move(mv));
        return *this;
    }
    
    
    using std::string::operator+=;
    
    String&     operator+=(char8_t ch)
    {
        std::string::operator+=((char) ch);
        return *this;
    }
    
    String&     operator+=(char32_t);
    
    using std::string::append;
    
    String&   append(size_t cnt, char8_t ch)
    {
        append(cnt, (char) ch);
        return *this;
    }
    
    String&   append(char ch)
    {
        append(1, ch);
        return *this;
    }
    
    String&   append(char8_t ch)
    {
        append(1, (char) ch);
        return *this;
    }
    
    String&             append(char32_t);
    String&             append(size_t, char32_t);
    
    
    /*! \brief Blanks C++ style comments
    */
    void                blank_cpp_comments();
    
    String              capitalize() const;
    
    size_t              char_count() const;
    
    size_t              count_of(char32_t) const;

   
    /*! \brief Filters a string with predicate
        \param Pred follows the syntax bool discard = p(whcar_t)
    */
    template <typename Pred>
    string_r            filtered(Pred p) const;

    template <typename Pred>
    string_r            filtered_w(Pred p) const;

    template <typename Pred>
    bool                for_each(Pred p) const;

    template <typename Pred>
    bool                for_each_abort(Pred p) const;

    template <typename Pred>
    bool                for_each_w(Pred p) const;
    
    Vector<String>      hard_wrap(size_t) const;
    
    /*! \brief Creates iterator for this string that'll work on char32_t
    
        \note   String must remain VALID/CONSTANT throughout this use!
    */
    Iter32              iterate() const;
    IterW               iterate_w() const;

    String              join(const Vector<String>&) const;
    String              join(const Set<String>&) const;
    String              join(const Set<String,IgCase>&) const;
    String              join(const List<String>&) const;
    
    String              lower() const;
    
    bool                matches_at(const String&, size_t pos=0) const;

    String              pad_left(size_t, char) const;
    String              pad_right(size_t, char) const;

    QByteArray          qBytes() const;
    QString             qString() const;
    
    /*! \brief Masks stuff between src/end with the target character
    */
    //bool    mask(const String& srcBeg, const String& srcEnd, chart tgt);
    bool                replace(char src, char tgt);
    
    /*! \brief used for Fusuion, sets if not-empty (left or right depends on override)
        
        \param b            Other string
        \param fOverride    If TRUE, will override if new is not empty, otherwise only if this is not empty
    */
    String&             set_if(const String&, bool fOverride=false);
    

    String              simplified() const;
    
    /*! \brief Splits the string into bits
    
        \param[in] ch           Character to split on
    */
    Vector<String>      split(char ch) const;
    Vector<String>      split(char ch, unsigned int) const;
    Vector<String>      split(const char*z) const;
    Vector<String>      split(const char*z,unsigned int) const;
    Vector<String>      split(const std::string& s) const;
    Vector<String>      split(const std::string& s,unsigned int) const;
    
    /*! \brief Removes ALL spaces */
    String              strip_spaces() const;

    bool                starts_with(const std::string&) const;
    bool                starts_with(const char*) const;
    
    String              trimmed() const;
    
    boolean_r           to_boolean() const;
    
    double_r            to_double() const;
    float_r             to_float() const;
    unsigned_r          to_hex() const;
    uint8_r             to_hex8() const;
    uint16_r            to_hex16() const;
    uint32_r            to_hex32() const;
    uint64_r            to_hex64() const;
    int8_r              to_int8() const;
    int16_r             to_int16() const;
    int32_r             to_int32() const;
    int64_r             to_int64() const;
    integer_r           to_integer() const;
    short_r             to_short() const;
    std::u8string       to_u8string() const;
    u32string_r         to_u32string() const;
    uint8_r             to_uint8() const;
    uint16_r            to_uint16() const;
    uint32_r            to_uint32() const;
    uint64_r            to_uint64() const;
    unsigned_r          to_uinteger() const;
    ushort_r            to_ushort() const;
    wstring_r           to_wstring() const;
    
    String              trim_end() const;

    String              upper() const;
    
    
private:
    String(const wchar_t*, const wchar_t*);
    String(const char32_t*, const char32_t*);
};

size_t      count_start_spaces(const String&);

Char32      first_non_blank_char(const String&);

String      first_non_empty(const String&, const String&);
String      first_non_empty(const String&, const String&, const String&);
String      first_non_empty(const String&, const String&, const String&, const String&);
String      first_non_empty(std::initializer_list<String>);

bool        is_blank(const String&);


bool        is_similar(const std::string&, const char*);
bool        is_similar(const std::string&, const std::string&);
//bool        is_similar(const QByteArray&, const QByteArray&);
bool        is_similar(const QString&, const QString&);
//bool	      isLessIgCase(const QByteArray&, const QByteArray&);
bool	    is_less_igCase(const std::string&, const std::string&);
//bool	      isLessIgCase(const std::string&, const std::string&);
//bool	      isGreaterIgCase(const QByteArray&, const QByteArray&);
//bool	      isGreaterIgCase(const std::string&, const std::string&);
bool        is_greater_igCase(const std::string&, const std::string&);

boolean_r   to_boolean(const char*);
boolean_r   to_boolean(const char*, size_t);
boolean_r   to_boolean(const std::string&);
boolean_r   to_boolean(const std::string_view&);
boolean_r   to_boolean(const QByteArray&);
boolean_r   to_boolean(const QString&);

double_r    to_double(const char*);
double_r    to_double(const char*, size_t);
double_r    to_double(const std::string&);
double_r    to_double(const std::string_view&);
double_r    to_double(const QByteArray&);
double_r    to_double(const QString&);

float_r     to_float(const char*);
float_r     to_float(const char*, size_t);
float_r     to_float(const std::string&);
float_r     to_float(const std::string_view&);
float_r     to_float(const QByteArray&);
float_r     to_float(const QString&);


unsigned_r  to_hex(const char*);
unsigned_r  to_hex(const char*, size_t);
unsigned_r  to_hex(const std::string&);
unsigned_r  to_hex(const std::string_view&);
unsigned_r  to_hex(const QByteArray&);
unsigned_r  to_hex(const QString&);

uint8_r     to_hex8(const char*);
uint8_r     to_hex8(const char*, size_t);
uint8_r     to_hex8(const std::string&);
uint8_r     to_hex8(const std::string_view&);
uint8_r     to_hex8(const QByteArray&);
uint8_r     to_hex8(const QString&);

uint16_r    to_hex16(const char*);
uint16_r    to_hex16(const char*, size_t);
uint16_r    to_hex16(const std::string&);
uint16_r    to_hex16(const std::string_view&);
uint16_r    to_hex16(const QByteArray&);
uint16_r    to_hex16(const QString&);

uint32_r    to_hex32(const char*);
uint32_r    to_hex32(const char*, size_t);
uint32_r    to_hex32(const std::string_view&);
uint32_r    to_hex32(const std::string&);
uint32_r    to_hex32(const QByteArray&);
uint32_r    to_hex32(const QString&);

uint64_r    to_hex64(const char*);
uint64_r    to_hex64(const char*, size_t);
uint64_r    to_hex64(const std::string&);
uint64_r    to_hex64(const std::string_view&);
uint64_r    to_hex64(const QByteArray&);
uint64_r    to_hex64(const QString&);

integer_r   to_int(const char*);
integer_r   to_int(const char*, size_t);
integer_r   to_int(const std::string&);
integer_r   to_int(const std::string_view&);
integer_r   to_int(const QByteArray&);
integer_r   to_int(const QString&);

int8_r      to_int8(const char*);
int8_r      to_int8(const char*, size_t);
int8_r      to_int8(const std::string&);
int8_r      to_int8(const std::string_view&);
int8_r      to_int8(const QByteArray&);
int8_r      to_int8(const QString&);

int16_r     to_int16(const char*);
int16_r     to_int16(const char*, size_t);
int16_r     to_int16(const std::string&);
int16_r     to_int16(const std::string_view&);
int16_r     to_int16(const QByteArray&);
int16_r     to_int16(const QString&);

int32_r     to_int32(const char*);
int32_r     to_int32(const char*, size_t);
int32_r     to_int32(const std::string&);
int32_r     to_int32(const std::string_view&);
int32_r     to_int32(const QByteArray&);
int32_r     to_int32(const QString&);

int64_r     to_int64(const char*);
int64_r     to_int64(const char*, size_t);
int64_r     to_int64(const std::string&);
int64_r     to_int64(const std::string_view&);
int64_r     to_int64(const QByteArray&);
int64_r     to_int64(const QString&);

integer_r   to_integer(const char*);
integer_r   to_integer(const char*, size_t);
integer_r   to_integer(const std::string&);
integer_r   to_integer(const std::string_view&);
integer_r   to_integer(const QByteArray&);
integer_r   to_integer(const QString&);

short_r     to_short(const char*);
short_r     to_short(const char*, size_t);
short_r     to_short(const std::string&);
short_r     to_short(const std::string_view&);
short_r     to_short(const QByteArray&);
short_r     to_short(const QString&);

unsigned_r  to_uint(const char*);
unsigned_r  to_uint(const char*, size_t);
unsigned_r  to_uint(const std::string_view&);
unsigned_r  to_uint(const std::string&);
unsigned_r  to_uint(const QByteArray&);
unsigned_r  to_uint(const QString&);

uint8_r     to_uint8(const char*);
uint8_r     to_uint8(const char*, size_t);
uint8_r     to_uint8(const std::string_view&);
uint8_r     to_uint8(const std::string&);
uint8_r     to_uint8(const QByteArray&);
uint8_r     to_uint8(const QString&);

uint16_r    to_uint16(const char*);
uint16_r    to_uint16(const char*, size_t);
uint16_r    to_uint16(const std::string&);
uint16_r    to_uint16(const std::string_view&);
uint16_r    to_uint16(const QByteArray&);
uint16_r    to_uint16(const QString&);

uint32_r    to_uint32(const char*);
uint32_r    to_uint32(const char*, size_t);
uint32_r    to_uint32(const std::string&);
uint32_r    to_uint32(const std::string_view&);
uint32_r    to_uint32(const QByteArray&);
uint32_r    to_uint32(const QString&);

uint64_r    to_uint64(const char*);
uint64_r    to_uint64(const char*, size_t);
uint64_r    to_uint64(const std::string_view&);
uint64_r    to_uint64(const std::string&);
uint64_r    to_uint64(const QByteArray&);
uint64_r    to_uint64(const QString&);

unsigned_r  to_uinteger(const char*);
unsigned_r  to_uinteger(const char*, size_t);
unsigned_r  to_uinteger(const std::string&);
unsigned_r  to_uinteger(const std::string_view&);
unsigned_r  to_uinteger(const QByteArray&);
unsigned_r  to_uinteger(const QString&);

unsigned_r  to_unsigned(const char*);
unsigned_r  to_unsigned(const char*, size_t);
unsigned_r  to_unsigned(const std::string&);
unsigned_r  to_unsigned(const std::string_view&);
unsigned_r  to_unsigned(const QByteArray&);
unsigned_r  to_unsigned(const QString&);

ushort_r    to_ushort(const char*);
ushort_r    to_ushort(const char*, size_t);
ushort_r    to_ushort(const std::string&);
ushort_r    to_ushort(const std::string_view&);
ushort_r    to_ushort(const QByteArray&);
ushort_r    to_ushort(const QString&);


class String::Join {
public:
    Join(const String&s) : m_separator(s), m_first(true) {}
    ~Join(){}
    
    operator String() const { return m_build; }
    const String&   str() const { return m_build; }

    template <typename T>
    Join&   append(const T& s)
    {
        if(m_first)
            m_first = false;
        else
            m_build += m_separator;
        m_build += s;
        return *this;
    }

    template <typename T>
    Join&   operator<<(const T& s)
    {
        append(s);
        return *this;
    }
    
    
private:
    String          m_separator;
    String          m_build;
    bool            m_first;
};


/*! \brief Used to iterate through the string as char32_t
    \note This stores a REFERENCE to the string!  and do NOT modify he string
*/
class String::Iter32 {
public:
    Iter32(const std::string& s) : Iter32(s.data(), s.size()) {}
    Iter32(const std::string_view& s) : Iter32(s.data(), s.size()) {}
    Iter32(const char*);
    Iter32(Iter32&& mv) = default;
    
    Iter32&   operator=(Iter32&&) = default;
    
    Char32      next();
    size_t      position() const;
    bool        done() const { return m_data >= m_end; }
    bool        more() const { return (m_data < m_end) && !m_error; }
    bool        error() const { return m_error; }
    
private:
    constexpr Iter32(const char*z, size_t n) :
        m_begin(z), 
        m_data(z), 
        m_end(z+n), 
        m_state{},
        m_error(false)
    {
    }
    
    Iter32(std::string&&) = delete;
    Iter32(std::string_view&&) = delete;
    Iter32(const Iter32&) = delete;
    Iter32& operator=(const Iter32&) = delete;
    
    const char*     m_begin;
    const char*     m_data;
    const char*     m_end;
    std::mbstate_t  m_state;
    bool            m_error;
};

/*! \brief Stores a REFERENCE to the string!
*/
class String::IterW {
public:

    IterW(const std::string& s);
    IterW(const std::string_view&);
    IterW(const char*);
    IterW(IterW&&);

    wchar_t     next();
    size_t      position() const;
    bool        done() const { return m_data >= m_end; }
    bool        more() const { return (m_data < m_end) && !m_error; }
    bool        error() const { return m_error; }

private:
    IterW(const char*, size_t);
    IterW(std::string&&) = delete;
    IterW(std::string_view&&) = delete;
    IterW(const Iter32&) = delete;
    
    const char*     m_begin;
    const char*     m_data;
    const char*     m_end;
    std::mbstate_t  m_state;
    bool            m_error;
};


//  


template <typename Pred>
string_r        String::filtered(Pred discard) const
{
    String      ret;
    ret.reserve(size());
    
    const char* p   = c_str();
    const char* e   = p + size();
    std::mbstate_t  state{};
    char32_t     wc = 0;
    int         len = 0;

    while((p < e) && ((len = std::mbrtoc32(&wc, p, e-p, &state)) > 0)){
        if(!discard(Char32(wc)))
            ret.append(p, len);
        p += len;
    }
    return string_r(p == e, ret);
}


template <typename Pred>
string_r        String::filtered_w(Pred discard) const
{
    String      ret;
    ret.reserve(size());

    const char* p   = c_str();
    const char* e   = p + size();
    std::mbstate_t  state{};
    wchar_t     wc = 0;
    int         len = 0;

    while((p < e) && ((len = std::mbrtowc(&wc, p, e-p, &state)) > 0)){
        if(!discard(wc))
            ret.append(p, len);
        p += len;
    }
    return string_r(p == e, ret);
}

template <typename Pred>
bool            String::for_each(Pred pred) const
{
    const char* p   = c_str();
    const char* e   = p + size();
    std::mbstate_t  state{};
    char32_t     wc = 0;
    int          len = 0;

    while((p < e) && ((len = std::mbrtoc32(&wc, p, e-p, &state)) > 0)){
        pred(Char32(wc));
        p += len;
    }
    return p == e;
}

template <typename Pred>
bool            String::for_each_abort(Pred pred) const
{
    const char* p   = c_str();
    const char* e   = p + size();
    std::mbstate_t  state{};
    char32_t     wc = 0;
    int          len = 0;

    while((p < e) && ((len = std::mbrtoc32(&wc, p, e-p, &state)) > 0)){
        if(!pred(Char32(wc)))
            break;
        p += len;
    }
    return p == e;
}


template <typename Pred>
bool            String::for_each_w(Pred pred) const
{
    const char* p   = c_str();
    const char* e   = p + size();
    std::mbstate_t  state{};
    wchar_t     wc = 0;
    int         len = 0;

    while((p < e) && ((len = std::mbrtowc(&wc, p, e-p, &state)) > 0)){
        pred(wc);
        p += len;
    }
    return p == e;
}


namespace std {
    /*! \brief Hash operator.
    
        This is needed for those wishing to use hash maps with ztype.
    */
    template<> 
    struct hash<String> : public hash<std::string> {
    };
}

