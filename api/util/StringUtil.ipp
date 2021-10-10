#pragma once

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
    StringView::Iter32  a(sa);
    StringView::Iter32  b(sb);
    
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

Compare compare_igCase(const std::string_view& sa, const std::string_view& sb)
{
    StringView::Iter32  a(sa);
    StringView::Iter32  b(sb);
    
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

size_t count_start_spaces(const StringView&a)
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

Char32  first_non_blank_char(const StringView&a)
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

StringView  first_non_empty(const StringView&a, const StringView&b)
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

StringView  first_non_empty(const StringView&a, const StringView&b, const StringView&c)
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

StringView  first_non_empty(const StringView&a, const StringView&b, const StringView&c, const StringView&d)
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

StringView  first_non_empty(std::initializer_list<StringView> args)
{
    for(const StringView& s : args)
        if(!s.empty())
            return s;
    return StringView();
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

bool    is_blank(const StringView&a)
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

bool    is_similar(const std::string_view&a, const char*b)
{
    return is_equal( compare_igCase(a, b));
}

bool    is_similar(const std::string&a, const std::string&b)
{
    return is_equal( compare_igCase(a, b));
}

bool    is_similar(const std::string_view&a, const std::string_view&b)
{
    return is_equal( compare_igCase(a, b));
}


bool    is_similar(const char*a, const char*b)
{
    return is_equal( compare_igCase(a, b));
}


bool    is_similar(const QString&a, const QString&b)
{
    return is_equal( compare_igCase(a, b));
    
}

bool	is_less_igCase(const std::string_view&a, const std::string_view&b)
{
    return is_less( compare_igCase(a,b));
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

bool    is_greater_igCase(const std::string_view&a, const std::string_view&b)
{
    return is_greater(compare_igCase(a,b));
}

