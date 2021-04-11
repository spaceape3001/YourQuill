////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Compare.hpp"
#include "List.hpp"
#include "NaN.hpp"
#include "Set.hpp"
#include "String.hpp"
#include "Vector.hpp"

#include <cctype>
#include <charconv>
#include <locale>
#include <optional>
#include <string.h>

#include <QByteArray>
#include <QString>

namespace {
    static constexpr const size_t   kStdBuf = 63;
};

    /*
        Sprinkled thorughout this source file is char <-> char8_t blind casts.  Data wise, this should be 
        fine, but technically this is implementation dependent because the standard is a bit...iffy here.
    */

//namespace {
    //const std::locale&    utf8()
    //{
        //static std::locale      s_utf8 = std::locale("en_US.UTF8");
        //return s_utf8;
    //}
//}

namespace {
    void    trim_ws(const char*& s, size_t& n)
    {
        while(s && *s && isspace(*s) && n)
            ++s, --n;
        while(s && n && isspace(s[n-1]))
            --n;
    }

    bool    is_same(const char*a, size_t n, const char *b)
    {
        if(a && b){
            for(;*a && *b && n; ++a, ++b, --n){
                if(::tolower(*a) != *b)
                    return false;
            }
            return !(*b || n);
        }
        return false;
    }

    static constexpr const auto bTRUE     = boolean_r( true, true );
    static constexpr const auto bFALSE    = boolean_r( true, false );
    static constexpr const auto bFAIL     = boolean_r(false, false);
    
    static constexpr const auto dFAIL     = double_r(false, NaN);
    static constexpr const auto dZERO     = double_r(false, 0.);
    
    static constexpr const auto fFAIL     = float_r(false, NaNf);
    static constexpr const auto fZERO     = float_r(false, 0.f);
    
    static constexpr const auto iFAIL     = int_r(false, 0);
    static constexpr const auto i8FAIL    = int8_r(false, 0);
    static constexpr const auto i16FAIL   = int16_r(false, 0);
    static constexpr const auto i32FAIL   = int32_r(false, 0);
    static constexpr const auto i64FAIL   = int64_r(false, 0);
    
    static constexpr const auto sFAIL     = short_r(false, 0);

    static constexpr const auto uFAIL     = unsigned_r(false, 0.);
    static constexpr const auto u8FAIL    = uint8_r(false, 0);
    static constexpr const auto u16FAIL   = uint16_r(false, 0);
    static constexpr const auto u32FAIL   = uint32_r(false, 0);
    static constexpr const auto u64FAIL   = uint64_r(false, 0);
    
    static constexpr const auto usFAIL    = ushort_r(false, 0);
    
    template <typename T>
    Result<T>   int_from_chars(const char*s, size_t n, int base=10)
    {
        T    res = 0;
        auto [p,ec] = std::from_chars(s, s+n, res, base);
        return Result<T>(ec == std::errc(), res);
    }
}

//#ifdef ENABLE_QT
    //QString     qString(const String&s)
    //{
        //return s.to_qstring().value;
    //}
    
    //QByteArray  qByteArray(const String&s)
    //{
        //return QByteArray(s.c_str());
    //}
    
//#endif

    //  ------------------------------------

boolean_r   to_boolean(const char*s)
{
    if(!s)
        return bFAIL;
    return to_boolean(s, strlen(s));
}

boolean_r   to_boolean(const char*s, size_t n)
{

    if(!s)
        return bFAIL;
    
    trim_ws(s, n);
    switch(*s){
    case '0':
        if(is_same(s,n,"0"))
            return bFALSE;
        break;
    case '1':
        if(is_same(s, n, "1"))
            return bTRUE;
        break;
    case 'y':
    case 'Y':
        if(n == 1)
            return bTRUE;
        if(is_same(s, n, "yes"))
            return bTRUE;
        break;
    case 'n':
    case 'N':
        if(n == 1)
            return bFALSE;
        if(is_same(s, n, "no"))
            return bFALSE;
        break;
    case 't':
    case 'T':
        if(n == 1)
            return bTRUE;
        if(is_same(s, n, "true"))
            return bTRUE;
        break;
    case 'f':
    case 'F':
        if(n==1)
            return bFALSE;
        if(is_same(s, n, "false"))
            return bFALSE;
        break;
    default:
        break;
    }
    
    return bFAIL;
}

boolean_r   to_boolean(const std::string&s)
{
    return to_boolean(s.data(), s.size());
}

boolean_r   to_boolean(const std::string_view&s)
{
    return to_boolean(s.data(), s.size());
}

boolean_r   to_boolean(const QByteArray&s)
{
    return to_boolean(s.constData(), s.size());
}

boolean_r   to_boolean(const QString&s)
{
    return to_boolean(s.toStdString());
}

    //  ------------------------------------

double_r    to_double(const char* s)
{
    if(!s)
        return dFAIL;
    return to_double(s, strlen(s));
}

double_r    to_double(const char*s, size_t n)
{
    if(!s)
        return dFAIL;
    trim_ws(s, n);
    if(!n)
        return 0.;

        //  Eventually "std::from_chars" will be available for floating point numbers in GCC ....
    char*   z       = nullptr;
    double  res     = std::strtod(s, &z);
    if((const char*) s != z)
        return res;
    return dFAIL;

    #if 0
    // std::from_chars ... nice according to the specification *BUT* gcc v10 does NOT support it for floating point types!!!! (Go figure)
    auto [p,ec] = std::from_chars((const char*) data(), (const char*)(data()+size()), result, std::chars_format::general);
    if(ok)
        *ok = (ec == std::errc());
    return result;
    #endif
}

double_r    to_double(const std::string&s)
{
    return to_double(s.data(), s.size());
}

double_r    to_double(const std::string_view&s)
{
    return to_double(s.data(), s.size());
}

double_r    to_double(const QByteArray&s)
{
    return to_double(s.constData(), s.size());
}

double_r    to_double(const QString&s)
{
    return to_double(s.toStdString());
}

    //  ------------------------------------

float_r     to_float(const char*s)
{
    if(!s)
        return fFAIL;
    return to_float(s, strlen(s));
}

float_r     to_float(const char*s, size_t n)
{
    if(!s)
        return fFAIL;
    trim_ws(s, n);
    if(!n)
        return 0.f;
    
        //  Eventually "std::from_chars" will be available for floating point numbers in GCC ....
    char*   z       = nullptr;
    float  res     = std::strtof(s, &z);
    if((const char*) s != z)
        return res;
    return fFAIL;

    #if 0
    // std::from_chars ... nice according to the specification *BUT* gcc v10 does NOT support it for floating point types!!!! (Go figure)
    float   result = NaNf;
    auto [p,ec] = std::from_chars(data(), data()+size(), result);
    if(ok)
        *ok = (ec == std::errc());
    return result;
    #endif
}

float_r     to_float(const std::string&s)
{
    return to_float(s.data(), s.size());
}

float_r     to_float(const std::string_view&s)
{
    return to_float(s.data(), s.size());
}

float_r     to_float(const QByteArray&s)
{
    return to_float(s.constData(), s.size());
}

float_r     to_float(const QString&s)
{
    return to_float(s.toStdString());
}

    //  ------------------------------------


unsigned_r  to_hex(const char*s)
{
    if(!s)
        return uFAIL;
    return to_hex(s, strlen(s));
}

unsigned_r  to_hex(const char*s, size_t n)
{
    if(!s)
        return uFAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<unsigned int>(s, n, 16);
}

unsigned_r  to_hex(const std::string&s)
{
    return to_hex(s.data(), s.size());
}

unsigned_r  to_hex(const std::string_view&s)
{
    return to_hex(s.data(), s.size());
}

unsigned_r  to_hex(const QByteArray&s)
{
    return to_hex(s.constData(), s.size());
}

unsigned_r  to_hex(const QString&s)
{
    return to_hex(s.toStdString());
}

    //  ------------------------------------

uint8_r     to_hex8(const char*s) 
{
    if(!s)  
        return u8FAIL;
    return to_hex8(s, strlen(s));
}

uint8_r     to_hex8(const char*s, size_t n)
{
    if(!s)  
        return u8FAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<uint8_t>(s, n, 16);
}

uint8_r     to_hex8(const std::string&s)
{
    return to_hex8(s.data(), s.size());
}

uint8_r     to_hex8(const std::string_view&s)
{
    return to_hex8(s.data(), s.size());
}

uint8_r     to_hex8(const QByteArray&s)
{
    return to_hex8(s.constData(), s.size());
}

uint8_r     to_hex8(const QString&s)
{
    return to_hex8(s.toStdString());
}


    //  ------------------------------------


uint16_r    to_hex16(const char*s)
{
    if(!s)
        return u16FAIL;
    return to_hex16(s, strlen(s));
}

uint16_r    to_hex16(const char*s, size_t n)
{
    if(!s)  
        return u16FAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<uint16_t>(s, n, 16);
}

uint16_r    to_hex16(const std::string&s)
{
    return to_hex16(s.data(), s.size());
}

uint16_r    to_hex16(const std::string_view&s)
{
    return to_hex16(s.data(), s.size());
}

uint16_r    to_hex16(const QByteArray&s)
{
    return to_hex16(s.constData(), s.size());
}

uint16_r    to_hex16(const QString&s)
{
    return to_hex16(s.toStdString());
}

    //  ------------------------------------

uint32_r    to_hex32(const char*s)
{
    if(!s)
        return u32FAIL;
    return to_hex32(s, strlen(s));
}

uint32_r    to_hex32(const char*s, size_t n)
{
    if(!s)
        return u32FAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<uint32_t>(s, n, 16);
}

uint32_r    to_hex32(const std::string_view&s)
{
    return to_hex32(s.data(), s.size());
}

uint32_r    to_hex32(const std::string&s)
{
    return to_hex32(s.data(), s.size());
}

uint32_r    to_hex32(const QByteArray&s)
{
    return to_hex32(s.constData(), s.size());
}

uint32_r    to_hex32(const QString&s)
{
    return to_hex32(s.toStdString());
}


    //  ------------------------------------

uint64_r    to_hex64(const char*s)
{
    if(!s)
        return u64FAIL;
    return to_hex64(s, strlen(s));
}

uint64_r    to_hex64(const char*s, size_t n)
{
    if(!s)
        return u64FAIL;
    trim_ws(s,n);
    if(!n)
        return 0ULL;
    return int_from_chars<uint64_t>(s, n, 16);
}

uint64_r    to_hex64(const std::string&s)
{
    return to_hex64(s.data(), s.size());
}

uint64_r    to_hex64(const std::string_view&s)
{
    return to_hex64(s.data(), s.size());
}

uint64_r    to_hex64(const QByteArray&s)
{
    return to_hex64(s.constData(), s.size());
}

uint64_r    to_hex64(const QString&s)
{
    return to_hex64(s.toStdString());
}


    //  ------------------------------------

integer_r   to_int(const char*s)
{
    if(!s)
        return iFAIL;
    return to_int(s,strlen(s));
}

integer_r   to_int(const char*s, size_t n)
{
    if(!s)
        return iFAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<int>(s,n);
}

integer_r   to_int(const std::string&s)
{
    return to_int(s.data(), s.size());
}

integer_r   to_int(const std::string_view&s)
{
    return to_int(s.data(), s.size());
}

integer_r   to_int(const QByteArray&s)
{
    return to_int(s.constData(), s.size());
}

integer_r   to_int(const QString&s)
{
    return to_int(s.toStdString());
}


    //  ------------------------------------

int8_r      to_int8(const char*s)
{
    if(!s)
        return i8FAIL;
    return to_int8(s,strlen(s));
}

int8_r      to_int8(const char*s, size_t n)
{
    if(!s)
        return i8FAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<int8_t>(s,n);
}

int8_r      to_int8(const std::string&s)
{
    return to_int8(s.data(), s.size());
}

int8_r      to_int8(const std::string_view&s)
{
    return to_int8(s.data(), s.size());
}

int8_r      to_int8(const QByteArray&s)
{
    return to_int8(s.constData(), s.size());
}

int8_r      to_int8(const QString&s)
{
    return to_int8(s.toStdString());
}


    //  ------------------------------------

int16_r     to_int16(const char*s)
{
    if(!s)
        return i16FAIL;
    return to_int16(s,strlen(s));
}

int16_r     to_int16(const char*s, size_t n)
{
    if(!s)
        return i16FAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<int16_t>(s,n);
}

int16_r     to_int16(const std::string&s)
{
    return to_int16(s.data(), s.size());
}

int16_r     to_int16(const std::string_view&s)
{
    return to_int16(s.data(), s.size());
}

int16_r     to_int16(const QByteArray&s)
{
    return to_int16(s.constData(), s.size());
}

int16_r     to_int16(const QString&s)
{
    return to_int16(s.toStdString());
}


    //  ------------------------------------

int32_r     to_int32(const char*s)
{
    if(!s)
        return i32FAIL;
    return to_int32(s,strlen(s));
}

int32_r     to_int32(const char*s, size_t n)
{
    if(!s)
        return i32FAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<int32_t>(s,n);
}

int32_r     to_int32(const std::string&s)
{
    return to_int32(s.data(), s.size());
}

int32_r     to_int32(const std::string_view&s)
{
    return to_int32(s.data(), s.size());
}

int32_r     to_int32(const QByteArray&s)
{
    return to_int32(s.constData(), s.size());
}

int32_r     to_int32(const QString&s)
{
    return to_int32(s.toStdString());
}

    //  ------------------------------------

int64_r     to_int64(const char*s)
{
    if(!s)
        return i64FAIL;
    return to_int64(s,strlen(s));
}

int64_r     to_int64(const char*s, size_t n)
{
    if(!s)
        return i64FAIL;
    trim_ws(s,n);
    if(!n)
        return 0LL;
    return int_from_chars<int64_t>(s,n);
}

int64_r     to_int64(const std::string&s)
{
    return to_int64(s.data(), s.size());
}

int64_r     to_int64(const std::string_view&s)
{
    return to_int64(s.data(), s.size());
}

int64_r     to_int64(const QByteArray&s)
{
    return to_int64(s.constData(), s.size());
}

int64_r     to_int64(const QString&s)
{
    return to_int64(s.toStdString());
}


    //  ------------------------------------

integer_r   to_integer(const char*s)
{
    if(!s)
        return iFAIL;
    return to_int(s,strlen(s));
}

integer_r   to_integer(const char*s, size_t n)
{
    if(!s)
        return iFAIL;
    trim_ws(s,n);
    if(!n)
        return 0L;
    return int_from_chars<int>(s,n);
}

integer_r   to_integer(const std::string&s)
{
    return to_integer(s.data(), s.size());
}

integer_r   to_integer(const std::string_view&s)
{
    return to_integer(s.data(), s.size());
}

integer_r   to_integer(const QByteArray&s)
{
    return to_integer(s.constData(), s.size());
}

integer_r   to_integer(const QString&s)
{
    return to_integer(s.toStdString());
}

    //  ------------------------------------

short_r     to_short(const char*s)
{
    if(!s)
        return sFAIL;
    return to_short(s,strlen(s));
}

short_r     to_short(const char*s, size_t n)
{
    if(!s)
        return sFAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<short>(s,n);
}

short_r     to_short(const std::string&s)
{
    return to_short(s.data(), s.size());
}

short_r     to_short(const std::string_view&s)
{
    return to_short(s.data(), s.size());
}

short_r     to_short(const QByteArray&s)
{
    return to_short(s.constData(), s.size());
}

short_r     to_short(const QString&s)
{
    return to_short(s.toStdString());
}

    //  ------------------------------------

unsigned_r  to_uint(const char*s)
{
    if(!s)
        return uFAIL;
    return to_uint(s,strlen(s));
}

unsigned_r  to_uint(const char*s, size_t n)
{
    if(!s)
        return uFAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<unsigned>(s,n);
}

unsigned_r  to_uint(const std::string&s)
{
    return to_uint(s.data(), s.size());
}

unsigned_r  to_uint(const std::string_view&s)
{
    return to_uint(s.data(), s.size());
}

unsigned_r  to_uint(const QByteArray&s)
{
    return to_uint(s.constData(), s.size());
}

unsigned_r  to_uint(const QString&s)
{
    return to_uint(s.toStdString());
}


    //  ------------------------------------

uint8_r     to_uint8(const char*s)
{
    if(!s)
        return u8FAIL;
    return to_uint8(s,strlen(s));
}

uint8_r     to_uint8(const char*s, size_t n)
{
    if(!s)
        return u8FAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<uint8_t>(s,n);
}

uint8_r     to_uint8(const std::string&s)
{
    return to_uint8(s.data(), s.size());
}

uint8_r     to_uint8(const std::string_view&s)
{
    return to_uint8(s.data(), s.size());
}

uint8_r     to_uint8(const QByteArray&s)
{
    return to_uint8(s.constData(), s.size());
}

uint8_r     to_uint8(const QString&s)
{
    return to_uint8(s.toStdString());
}

    //  ------------------------------------

uint16_r    to_uint16(const char*s)
{
    if(!s)
        return u16FAIL;
    return to_uint16(s,strlen(s));
}

uint16_r    to_uint16(const char*s, size_t n)
{
    if(!s)
        return u16FAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<uint16_t>(s,n);
}

uint16_r    to_uint16(const std::string&s)
{
    return to_uint16(s.data(), s.size());
}

uint16_r    to_uint16(const std::string_view&s)
{
    return to_uint16(s.data(), s.size());
}

uint16_r    to_uint16(const QByteArray&s)
{
    return to_uint16(s.constData(), s.size());
}

uint16_r    to_uint16(const QString&s)
{
    return to_uint16(s.toStdString());
}

    //  ------------------------------------

uint32_r    to_uint32(const char*s)
{
    if(!s)
        return u32FAIL;
    return to_uint32(s,strlen(s));
}

uint32_r    to_uint32(const char*s, size_t n)
{
    if(!s)
        return u32FAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<uint32_t>(s,n);
}

uint32_r    to_uint32(const std::string&s)
{
    return to_uint32(s.data(), s.size());
}

uint32_r    to_uint32(const std::string_view&s)
{
    return to_uint32(s.data(), s.size());
}

uint32_r    to_uint32(const QByteArray&s)
{
    return to_uint32(s.constData(), s.size());
}

uint32_r    to_uint32(const QString&s)
{
    return to_uint32(s.toStdString());
}


    //  ------------------------------------

uint64_r    to_uint64(const char*s)
{
    if(!s)
        return u64FAIL;
    return to_uint64(s,strlen(s));
}

uint64_r    to_uint64(const char*s, size_t n)
{
    if(!s)
        return u64FAIL;
    trim_ws(s,n);
    if(!n)
        return 0ULL;
    return int_from_chars<uint64_t>(s,n);
}

uint64_r    to_uint64(const std::string&s)
{
    return to_uint64(s.data(), s.size());
}

uint64_r    to_uint64(const std::string_view&s)
{
    return to_uint64(s.data(), s.size());
}

uint64_r    to_uint64(const QByteArray&s)
{
    return to_uint64(s.constData(), s.size());
}

uint64_r    to_uint64(const QString&s)
{
    return to_uint64(s.toStdString());
}



    //  ------------------------------------

unsigned_r  to_uinteger(const char*s)
{
    if(!s)
        return uFAIL;
    return to_uinteger(s,strlen(s));
}

unsigned_r  to_uinteger(const char*s, size_t n)
{
    if(!s)
        return uFAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<unsigned int>(s,n);
}

unsigned_r  to_uinteger(const std::string&s)
{
    return to_uinteger(s.data(), s.size());
}

unsigned_r  to_uinteger(const std::string_view&s)
{
    return to_uinteger(s.data(), s.size());
}

unsigned_r  to_uinteger(const QByteArray&s)
{
    return to_uinteger(s.constData(), s.size());
}

unsigned_r  to_uinteger(const QString&s)
{
    return to_uinteger(s.toStdString());
}



    //  ------------------------------------

unsigned_r  to_unsigned(const char*s)
{
    if(!s)
        return uFAIL;
    return to_uint(s,strlen(s));
}

unsigned_r  to_unsigned(const char*s, size_t n)
{
    if(!s)
        return uFAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<unsigned int>(s,n);
}

unsigned_r  to_unsigned(const std::string&s)
{
    return to_unsigned(s.data(), s.size());
}

unsigned_r  to_unsigned(const std::string_view&s)
{
    return to_unsigned(s.data(), s.size());
}

unsigned_r  to_unsigned(const QByteArray&s)
{
    return to_unsigned(s.constData(), s.size());
}

unsigned_r  to_unsigned(const QString&s)
{
    return to_unsigned(s.toStdString());
}



    //  ------------------------------------

ushort_r    to_ushort(const char*s)
{
    if(!s)
        return usFAIL;
    return to_ushort(s,strlen(s));
}

ushort_r    to_ushort(const char*s, size_t n)
{
    if(!s)
        return usFAIL;
    trim_ws(s,n);
    if(!n)
        return 0;
    return int_from_chars<unsigned short>(s,n);
}

ushort_r    to_ushort(const std::string&s)
{
    return to_ushort(s.data(), s.size());
}

ushort_r    to_ushort(const std::string_view&s)
{
    return to_ushort(s.data(), s.size());
}

ushort_r    to_ushort(const QByteArray&s)
{
    return to_ushort(s.constData(), s.size());
}

ushort_r    to_ushort(const QString&s)
{
    return to_ushort(s.toStdString());
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bool        Char8::is_alnum() const
{
    return ::isalnum(m_code);
}

bool        Char8::is_alpha() const
{
    return ::isalpha(m_code);
}

bool        Char8::is_blank() const
{
    return ::isblank(m_code);
}

bool        Char8::is_control() const
{
    return ::iscntrl(m_code);
}

bool        Char8::is_digit() const
{
    return ::isdigit(m_code);
}

bool        Char8::is_graph() const
{
    return ::isgraph(m_code);
}

bool        Char8::is_lower() const
{
    return ::islower(m_code);
}

bool        Char8::is_print() const
{
    return ::isprint(m_code);
}

bool        Char8::is_punct() const
{
    return ::ispunct(m_code);
}

bool        Char8::is_space() const
{
    return ::isspace(m_code);
}

bool        Char8::is_upper() const
{
    return ::isupper(m_code);
}

bool        Char8::is_xdigit() const
{
    return ::isxdigit(m_code);
}

char        Char8::to_lower() const
{
    return ::tolower(m_code);
}

char        Char8::to_upper() const
{
    return ::toupper(m_code);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


bool        Char32::is_alnum() const
{
    return ::iswalnum(m_code);
}

bool        Char32::is_alpha() const
{
    return ::iswalpha(m_code);
}

bool        Char32::is_blank() const
{
    return ::iswblank(m_code);
}

bool        Char32::is_control() const
{
    return ::iswcntrl(m_code);
}

bool        Char32::is_digit() const
{
    return ::iswdigit(m_code);
}

bool        Char32::is_graph() const
{
    return ::iswgraph(m_code);
}

bool        Char32::is_lower() const
{
    return ::iswlower(m_code);
}

bool        Char32::is_print() const
{
    return ::iswprint(m_code);
}

bool        Char32::is_punct() const
{
    return ::iswpunct(m_code);
}

bool        Char32::is_space() const
{
    return ::iswspace(m_code);
}

bool        Char32::is_upper() const
{
    return ::iswupper(m_code);
}

bool        Char32::is_xdigit() const
{
    return ::iswxdigit(m_code);
}

char32_t    Char32::to_lower() const
{
    return ::towlower(m_code);
}

char32_t    Char32::to_upper() const
{
    return ::towupper(m_code);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

String   String::boolean(bool v, const String& kFalse, const String& kTrue)
{
    return v ? kTrue : kFalse;
}

String   String::hex(uint8_t n, char f)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, n, 16);
    return String(buf, (p-buf)).pad_left(2, f);
}

String   String::hex(uint16_t n, char f)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, n, 16);
    return String(buf, (p-buf)).pad_left(4, f);
}

String   String::hex(uint32_t n, char f)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, n, 16);
    return String(buf, (p-buf)).pad_left(8, f);
}

String   String::hex(uint64_t n, char f)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, n, 16);
    return String(buf, (p-buf)).pad_left(16, f);
}

String   String::number(double v)
{
    //  std::to_chars would be GREAT, if GCC implemented it.....
    char    buf[kStdBuf+1];
    int n  = snprintf(buf, kStdBuf, "%lg", v);
    return String(buf, n);
}

String   String::number(float v)
{
    //  std::to_chars would be GREAT, if GCC implemented it.....
    char    buf[kStdBuf+1];
    int n  = snprintf(buf, kStdBuf, "%g", v);
    return String(buf, n);
}


String   String::number(int8_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    return String(buf, (p-buf));
}

String   String::number(int16_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    return String(buf, (p-buf));
}

String   String::number(int32_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    return String(buf, (p-buf));
}

String   String::number(int64_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    return String(buf, (p-buf));
}

String   String::number(uint8_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    return String(buf, (p-buf));
}

String   String::number(uint16_t v) 
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    return String(buf, (p-buf));
}

String   String::number(uint32_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    return String(buf, (p-buf));
}

String   String::number(uint64_t v)
{
    char    buf[kStdBuf+1];
    auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
    return String(buf, (p-buf));
}

String   String::scientific(double v, int digits)
{
    //  std::to_chars would be GREAT, if GCC implemented it.....
    char    buf[kStdBuf+1];
    int n  = snprintf(buf, kStdBuf, "%0.*le", digits, v);
    return String(buf, n);
}

String   String::scientific(float v, int digits)
{
    //  std::to_chars would be GREAT, if GCC implemented it.....
    char    buf[kStdBuf+1];
    int n  = snprintf(buf, kStdBuf, "%0.*e", digits, v);
    return String(buf, n);
}


// ---------------------------------------------------------------------------------------------------------------------


String::String(char32_t ch)
{
    append(ch);
}



String::String(const char32_t*z)
{
    for(;z && *z; ++z)
        append(*z);
}



String::String(const std::u8string&z) : std::string(z.begin(), z.end())   // FIXME
{
}

String::String(const std::u8string&z, size_type pos, size_type count) : std::string((const char*) z.c_str(), pos, count)
{
}


String::String(const std::u8string_view&z) : std::string(z.begin(), z.end())
{
}


String::String(const char32_t*a, const char32_t*b)
{
    reserve(3*(b-a));
    std::mbstate_t  state{};
    char            buffer[MB_CUR_MAX+1];
    for(;a<b;++a){
        int len = (int) c32rtomb(buffer, *a, &state);
        if(len < 0)
            break;
        append(buffer, len);
    }
}

String::String(const std::u32string& a) : String(a.data(), a.data()+a.size())
{
}

String::String(const std::u32string_view& a) : String(a.data(), a.data()+a.size())
{
}


String::String(const wchar_t*a, const wchar_t*b)
{
    reserve(3*(b-a));
    std::mbstate_t  state{};
    char            buffer[MB_CUR_MAX+1];
    for(;a<b;++a){
        int len = (int) wcrtomb(buffer, *a, &state);
        if(len < 0)
            break;
        append(buffer, len);
    }
}


String::String(const std::wstring& a) : String(a.data(), a.data()+a.size())
{
}

String::String(const std::wstring_view&a) : String(a.data(), a.data()+a.size())
{
}

String::String(const QString&s) : String(s.toStdU32String())
{
}

String::String(const QByteArray&s) : String(s.constData(), s.size())
{
}


String::String(const Vector<uint8_t>&data) : String((const char*) data.data(), data.size())
{
}


String& String::operator=(const std::u8string&z)
{
    std::string::operator=(std::string(z.begin(), z.end()));
    return *this;
}

String& String::operator=(const std::u8string_view&z)
{
    std::string::operator=(String(z));
    return *this;
}

String& String::operator=(const std::u32string_view& z)
{
    std::string::operator=(String(z));
    return *this;
}

String& String::operator=(const std::wstring_view&z)
{
    std::string::operator=(String(z));
    return *this;
}


String&   String::operator=(const QByteArray&z)
{
    std::string::operator=(z.constData());
    return *this;
}

String&   String::operator=(const QString&z)
{
    std::string::operator=(String(z));
    return *this;
}



String& String::operator+=(char32_t ch)
{
    append(ch);
    return *this;
}


String& String::append(char32_t ch)
{
    std::mbstate_t  state{};
    char            buffer[MB_CUR_MAX+1];
    int len = (int) c32rtomb(buffer, ch, &state);
    if(len > 0)
        append(buffer, len);
    return *this;
}

String& String::append(size_t n, char32_t ch)
{
    while(n--)
        append(ch);
    return *this;
}


void    String::blank_cpp_comments()
{
    const size_t kSize = size();
    for(size_t i=0;i<kSize;++i){
        if((at(i) == '/') && (at(i+1) == '/')){
            for(; (i<kSize) && (at(i) != '\n'); ++i)
                at(i)   = ' ';
        }
    }
}

String          String::capitalize() const
{
    bool        space = true;
    String      ret;
    for_each([&](Char32 c){
        if(c.is_space()){
            space   = true;
            ret += c;
        } else {
            if(c.is_alpha()){
                if(space)
                    ret += c.to_upper();
                else
                    ret += c.to_lower();
            } else 
                ret += c;
            space   = false;
        }
    });
    return ret;
}


size_t          String::char_count() const
{
    size_t  cnt = 0;
    for_each([&](Char32){
        ++cnt;
    });
    return cnt;
}

size_t          String::count_of(char32_t ch) const
{
    size_t  cnt = 0;
    for_each([&](char32_t c){
        if(c == ch)
            ++cnt;
    });
    return cnt;
}

Vector<String>  String::hard_wrap(size_t n) const
{
    Vector<String>  ret;
    String          buf;
    size_t          m   = 0;
    for_each([&](char32_t ch){
        if(m>n){
            ret << buf;
            buf.clear();
        }
        buf += ch;
    });
    if(!buf.empty())
        ret << buf;
    return ret;
}

String::Iter32          String::iterate() const
{
    return Iter32(*this);
}

String::IterW           String::iterate_w() const
{
    return IterW(*this);
}

String          String::join(const Vector<String>& v) const
{
    String      ret;
    bool        first=true;
    for(const String& s : v){
        if(first)
            first   = false;
        else
            ret.append(*this);
        ret.append(s);
    }
    return ret;
}

String          String::join(const Set<String>&v) const
{
    String      ret;
    bool        first=true;
    for(const String& s : v){
        if(first)
            first   = false;
        else
            ret.append(*this);
        ret.append(s);
    }
    return ret;
}

String          String::join(const Set<String,IgCase>&v) const
{
    String      ret;
    bool        first=true;
    for(const String& s : v){
        if(first)
            first   = false;
        else
            ret.append(*this);
        ret.append(s);
    }
    return ret;
}

String          String::join(const List<String>&v) const
{
    String      ret;
    bool        first=true;
    for(const String& s : v){
        if(first)
            first   = false;
        else
            ret.append(*this);
        ret.append(s);
    }
    return ret;
}


String          String::lower() const
{
    String      ret;
    for_each([&](Char32 c){
        ret += c.to_lower();
    });
    return ret;
}


//bool    String::mask(const String& srcBeg, const String& srcEnd, char tgt)
//{
    //if(srcBeg.empty() || srcEnd.empty())
        //return false;
    //bool    ret = false;
    //for(size_t i=0;i<kSize;++i){
        //if(starts(srcBeg, i))
    //}
//}

bool    String::matches_at(const String& txt, size_t pos) const
{
    size_t      jSize   = txt.size();
    size_t      kSize   = size();
    if(jSize + pos > kSize)
        return false;
        
    for(size_t i=0;i<txt.size();++i)
        if(txt.at(i) != at(pos+i))
            return false;
    return true;
}


String  String::pad_left(size_t n, char ch) const
{
    if(size() >= n)
        return *this;
    return String(n-size(), ch) + *this;
}


String  String::pad_right(size_t n, char ch) const
{
    if(size() >= n)
        return *this;
    return *this + String(n-size(),ch);
}

QByteArray  String::qBytes() const
{
    return QByteArray(c_str(), size());
}

QString     String::qString() const
{
    return QString::fromStdU32String(to_u32string().value);
}


bool    String::replace(char src, char tgt)
{
    bool    ret = false;
    const size_t kSize = size();
    for(size_t i=0;i<kSize;++i)
        if(at(i) == src){
            at(i) = tgt;
            ret = true;
        }
    return ret;
}

String& String::set_if(const String& b, bool fOverride)
{
    if(!b.empty()){
        if(fOverride || empty())
            operator=(b);
    }
    return *this;
}

String          String::simplified() const
{
    String  ret;
    bool    space   = true;
    bool    first   = true;
    for_each([&](Char32 c){
        if(c.is_space()){
            space  = true;
        } else {
            if(first)
                first   = false;
            else
                ret.append(' ');
            ret.append(c);
            space = false;
        }
    });
    return ret;
}

Vector<String>  String::split(char ch) const
{
    Vector<String>  ret;
    if(!empty()){
        size_type   i = 0;
        size_type   j = 0;
        for(;(j = find(ch, i)) != npos; i = j+1)
            ret << String(*this, i, j-i);
        ret << String(*this, i);
    }
    return ret;
}

Vector<String>      String::split(char ch, unsigned int n) const
{
    Vector<String>  ret;
    if(!empty()){
        size_type   i = 0;
        size_type   j = 0;
        for(;n-- && (j = find(ch, i)) != npos; i = j+1)
            ret << String(*this, i, j-i);
        ret << String(*this, i);
    }
    return ret;
}


Vector<String>  String::split(const char*z) const
{
    Vector<String>  ret;
    if(!empty()){
        size_type   i = 0;
        size_type   j = 0;
        for(;(j = find(z, i)) != npos; i = j+1)
            ret << String(*this, i, j-i);
        ret << String(*this, i);
    }
    return ret;
}

Vector<String>  String::split(const char*z, unsigned int n) const
{
    Vector<String>  ret;
    if(!empty()){
        size_type   i = 0;
        size_type   j = 0;
        for(;n-- && (j = find(z, i)) != npos; i = j+1)
            ret << String(*this, i, j-i);
        ret << String(*this, i);
    }
    return ret;
}

Vector<String>  String::split(const std::string& s) const
{
    return split(s.c_str());
}

Vector<String>  String::split(const std::string&z, unsigned int n) const
{
    Vector<String>  ret;
    if(!empty()){
        size_type   i = 0;
        size_type   j = 0;
        for(;n-- && (j = find(z, i)) != npos; i = j+1)
            ret << String(*this, i, j-i);
        ret << String(*this, i);
    }
    return ret;
}

bool            String::starts_with(const std::string&s) const
{
    if(s.size() > size())   
        return false;
    for(size_t i = 0; i < s.size(); ++i)
        if(operator[](i) != s[i])
            return false;
    return true;
}

bool            String::starts_with(const char*s) const
{
    if(!s)
        return false;
    size_t  i=0;
    for(;i<size() && s[i]; ++i)
        if(operator[](i) != s[i])
            return false;
    return !s[i];
}

String          String::strip_spaces() const
{
    return filtered([](Char32 ch) -> bool { return ch.is_space(); });
}

boolean_r       String::to_boolean() const
{
    return ::to_boolean(c_str(), size());
}

double_r        String::to_double() const
{
    return ::to_double(c_str(), size());
}

float_r           String::to_float() const
{
    return ::to_float(c_str(), size());
}

unsigned_r    String::to_hex() const
{
    return ::to_hex(c_str(), size());
}

uint8_r         String::to_hex8() const
{
    return ::to_hex8(c_str(), size());
}

uint16_r        String::to_hex16() const
{
    return ::to_hex16(c_str(), size());
}

uint32_r        String::to_hex32() const
{
    return ::to_hex32(c_str(), size());
}

uint64_r        String::to_hex64() const
{
    return ::to_hex64(c_str(), size());
}

int8_r          String::to_int8() const
{
    return ::to_int8(c_str(), size());
}

int16_r         String::to_int16() const
{
    return ::to_int16(c_str(), size());
}

int32_r         String::to_int32() const
{
    return ::to_int32(c_str(), size());
}

int64_r         String::to_int64() const
{
    return ::to_int64(c_str(), size());
}

integer_r      String::to_integer() const
{
    return ::to_integer(c_str(), size());
}

short_r         String::to_short() const
{
    return ::to_short(c_str(), size());
}

std::u8string   String::to_u8string() const
{
    return std::u8string((const char8_t*) c_str(), size());
}


u32string_r    String::to_u32string() const
{
    std::u32string    ret;
    ret.reserve(size());
    bool ok = for_each([&](char32_t c){
        ret += c;
    });
    return u32string_r(ok, ret);
}

uint8_r         String::to_uint8() const
{
    return ::to_uint8(c_str(), size());
}

uint16_r        String::to_uint16() const
{
    return ::to_uint16(c_str(), size());
}

uint32_r        String::to_uint32() const
{
    return ::to_uint32(c_str(), size());
}

uint64_r        String::to_uint64() const
{
    return ::to_uint64(c_str(), size());
}


unsigned_r      String::to_uinteger() const
{
    return ::to_uinteger(c_str(), size());
}

ushort_r  String::to_ushort() const
{
    return ::to_ushort(c_str(), size());
}

wstring_r    String::to_wstring() const
{
    std::wstring    ret;
    ret.reserve(size());
    
    bool ok = for_each_w([&](wchar_t c) {
        ret += c;
    });
    return wstring_r(ok, ret);
}

String          String::trim_end() const
{
    String  ret;
    String  space;
    ret.reserve(size());
    
    for_each([&](Char32 ch){
        if(ch.is_space()){
            space += ch;
        } else {
            if(!space.empty()){
                ret += space;
                space.clear();
            }
            ret += ch;
        }
    });
    return ret;
}

String          String::trimmed() const
{
    String      ret;
    String      space;
    ret.reserve(size());
    bool        start   = false;
    
    for_each([&](Char32 ch){
        if(ch.is_space()){
            if(start)
                space += ch;
        } else {
            start   = true;
            if(!space.empty()){
                ret += space;
                space.clear();
            }
            ret += ch;
        }
    });
    return ret;
}

String          String::upper() const
{
    String      ret;
    for_each([&](Char32 c){
        ret += c.to_upper();
    });
    return ret;
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


String::Iter32::Iter32(const char*z) : Iter32(z, strlen((const char*) z))
{
}


size_t      String::Iter32::position() const 
{ 
    return m_data - m_begin; 
}

Char32      String::Iter32::next()
{
    char32_t ret = 0;
    if((m_data < m_end) && !m_error){
        int len = std::mbrtoc32(&ret, m_data, m_end-m_data, &m_state);
        if(len > 0){
            m_data     += len;
        } else {
            m_error     = true;
        }
    }
    return ret;
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

String::IterW::IterW(const char*z, size_t n) : m_begin(z), m_data(z), m_end(z+n), m_state{}, m_error(false)
{
}

String::IterW::IterW(const std::string& s) : IterW(s.data(), s.size())
{
}

String::IterW::IterW(const std::string_view&s) : IterW(s.data(), s.size())
{
}

String::IterW::IterW(const char*z) : IterW(z, strlen((const char*) z))
{
}

size_t      String::IterW::position() const 
{ 
    return m_data - m_begin; 
}

wchar_t    String::IterW::next()
{
    wchar_t ret = 0;
    if((m_data < m_end) && !m_error){
        int len = std::mbrtowc(&ret, m_data, m_end - m_data, &m_state);
        if(len > 0){
            m_data  += len;
        } else {
            m_error = true;
        }
    }
    return ret;
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Compare compare(const QString& a, const QString& b)
{
    int r   = QString::compare(a,b,Qt::CaseInsensitive);
    if(r<0)
        return Compare::LESSER;
    if(r>0)
        return Compare::GREATER;
    return Compare::EQUAL;
}

Compare compare(const std::string& a, const std::string& b)
{
    size_t  c   = std::min(a.size(), b.size());
    for(size_t i=0;i<c;++i){
        if(a[i] < b[i])
            return Compare::LESSER;
        if(a[i] > b[i])
            return Compare::GREATER;
    }
    
    return compare(a.size(), b.size());
}

Compare compare_igCase(const char*a, const char* b)
{
    if(!a && b)
        return Compare::LESSER;
    if(a && !b)
        return Compare::GREATER;
    if(!a && !b)
        return Compare::EQUAL;
        
    int r   = 0;
    for(; *a && *b; ++a, ++b){
        r   = tolower(*a) - tolower(*b);
        if(r)
            break;
    }
    
    if(!r)
        r = (int) *a - (int) *b;
        
        
    if(r<0)
        return Compare::LESSER;
    if(r>0)
        return Compare::GREATER;
    return Compare::EQUAL;
}


Compare compare_igCase(const QByteArray& a, const QByteArray& b)
{
    int r   = a.compare(b,Qt::CaseInsensitive);
    if(r<0)
        return Compare::LESSER;
    if(r>0)
        return Compare::GREATER;
    return Compare::EQUAL;
}

Compare compare_igCase(const QString& a, const QString& b)
{
    int r   = QString::compare(a,b,Qt::CaseInsensitive);
    if(r<0)
        return Compare::LESSER;
    if(r>0)
        return Compare::GREATER;
    return Compare::EQUAL;
}


Compare compare_igCase(const std::string& sa, const std::string& sb)
{
    String::Iter32  a(sa);
    String::Iter32  b(sb);
    
    for(;;){
        Char32  ca  = a.next();
        Char32  cb  = b.next();
        
        int64_t n   = (int64_t) ca.to_lower() - (int64_t) cb.to_lower();
        if( n != 0){
            if(n<0)
                return Compare::LESSER;
            else if(n>0)
                return Compare::GREATER;
        }
        if(ca.code() == 0)
            return Compare::EQUAL;
    }
}

size_t count_start_spaces(const String&a)
{
    size_t  i   = 0;
    a.for_each_abort([&](Char32 ch) -> bool {
        if(ch.is_space()){
            ++i;
            return true;
        } else
            return false;
    });
    return i;
}

Char32  first_non_blank_char(const String&a)
{
    Char32  ret;
    a.for_each_abort([&](Char32 ch) -> bool {
        if(!ch.is_space()){
            ret = ch;
            return false;
        }
        return true;
    });
    return ret;
}


String  first_non_empty(const String&a, const String&b)
{
    return a.empty() ? b : a;
}

String  first_non_empty(const String&a, const String&b, const String&c)
{
    if(!a.empty())
        return a;
    if(!b.empty())
        return b;
    return c;
}

String  first_non_empty(const String&a, const String&b, const String&c, const String&d)
{
    if(!a.empty())
        return a;
    if(!b.empty())
        return b;
    if(!c.empty())
        return c;
    return d;
}

String  first_non_empty(std::initializer_list<String> args)
{
    for(const String& s : args)
        if(!s.empty())
            return s;
    return String();
}

bool    is_blank(const String&a)
{
    bool    ret = true;
    a.for_each([&](Char32 ch) {
        if(!ch.is_space())
            ret = false;
    });
    return ret;
}

bool    is_similar(const std::string&a, const char*b)
{
    return is_equal( compare_igCase(a, b));
}

bool    is_similar(const std::string&a, const std::string&b)
{
    return is_equal( compare_igCase(a, b));
    
}

bool    is_similar(const QString&a, const QString&b)
{
    return is_equal( compare_igCase(a, b));
    
}


bool	is_less_igCase(const std::string&a, const std::string&b)
{
    return is_less( compare_igCase(a,b));
}

bool	is_less_igCase(const QString&a, const QString&b)
{
    return is_less( compare_igCase(a,b));
}

bool    is_greater_igCase(const std::string&a, const std::string&b)
{
    return is_greater(compare_igCase(a,b));
}


bool    IgCase::operator()(const String&a, const String&b) const
{
    return is_less( compare_igCase(a,b));
}

bool    IgCase::operator()(const QString&a, const QString&b) const
{
    return is_less( compare_igCase(a,b));
}

bool    IgCase::operator()(const QByteArray&a, const QByteArray&b) const
{
    return is_less( compare_igCase(a,b));
}


bool    RevIgCase::operator()(const String&a, const String&b) const
{
    return is_greater( compare_igCase(a,b));
}

bool    RevIgCase::operator()(const QString&a, const QString&b) const
{
    return is_greater( compare_igCase(a,b));
}

bool    RevIgCase::operator()(const QByteArray&a, const QByteArray&b) const
{
    return is_greater( compare_igCase(a,b));
}
