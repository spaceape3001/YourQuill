#pragma once

size_t          StringView::char_count() const
{
    size_t  cnt = 0;
    for_each([&](Char32){
        ++cnt;
    });
    return cnt;
}

size_t          StringView::count_of(char32_t ch) const
{
    size_t  cnt = 0;
    for_each([&](char32_t c){
        if(c == ch)
            ++cnt;
    });
    return cnt;
}

StringView::Iter32          StringView::iterate() const
{
    return Iter32(*this);
}

StringView::IterW           StringView::iterate_w() const
{
    return IterW(*this);
}


bool    StringView::matches_at(const StringView& txt, size_t pos) const
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

QByteArray  StringView::qBytes() const
{
    return QByteArray(data(), size());
}

QString     StringView::qString() const
{
    return QString::fromStdU32String(to_u32string().value);
}


Vector<StringView>  StringView::split(char ch) const
{
    Vector<StringView>  ret;
    if(!empty()){
        size_type   i = 0;
        size_type   j = 0;
        for(;(j = find(ch, i)) != npos; i = j+1)
            ret << StringView(*this, i, j-i);
        ret << StringView(*this, i);
    }
    return ret;
}

Vector<StringView>      StringView::split(char ch, unsigned int n) const
{
    Vector<StringView>  ret;
    if(!empty()){
        size_type   i = 0;
        size_type   j = 0;
        for(;n-- && (j = find(ch, i)) != npos; i = j+1)
            ret << StringView(*this, i, j-i);
        ret << StringView(*this, i);
    }
    return ret;
}


Vector<StringView>  StringView::split(const char*z) const
{
    Vector<StringView>  ret;
    if(!empty()){
        size_type   i = 0;
        size_type   j = 0;
        for(;(j = find(z, i)) != npos; i = j+1)
            ret << StringView(*this, i, j-i);
        ret << StringView(*this, i);
    }
    return ret;
}

Vector<StringView>  StringView::split(const char*z, unsigned int n) const
{
    Vector<StringView>  ret;
    if(!empty()){
        size_type   i = 0;
        size_type   j = 0;
        for(;n-- && (j = find(z, i)) != npos; i = j+1)
            ret << StringView(*this, i, j-i);
        ret << StringView(*this, i);
    }
    return ret;
}

Vector<StringView>  StringView::split(const std::string& s) const
{
    return split(s.c_str());
}

Vector<StringView>  StringView::split(const std::string&z, unsigned int n) const
{
    Vector<StringView>  ret;
    if(!empty()){
        size_type   i = 0;
        size_type   j = 0;
        for(;n-- && (j = find(z, i)) != npos; i = j+1)
            ret << StringView(*this, i, j-i);
        ret << StringView(*this, i);
    }
    return ret;
}

bool            StringView::starts_with(const std::string&s) const
{
    if(s.size() > size())   
        return false;
    for(size_t i = 0; i < s.size(); ++i)
        if(operator[](i) != s[i])
            return false;
    return true;
}

bool            StringView::starts_with(const char*s) const
{
    if(!s)
        return false;
    size_t  i=0;
    for(;i<size() && s[i]; ++i)
        if(operator[](i) != s[i])
            return false;
    return !s[i];
}

boolean_r       StringView::to_boolean() const
{
    return ::to_boolean(data(), size());
}

double_r        StringView::to_double() const
{
    return ::to_double(data(), size());
}

float_r         StringView::to_float() const
{
    return ::to_float(data(), size());
}

unsigned_r      StringView::to_hex() const
{
    return ::to_hex(data(), size());
}

uint8_r         StringView::to_hex8() const
{
    return ::to_hex8(data(), size());
}

uint16_r        StringView::to_hex16() const
{
    return ::to_hex16(data(), size());
}

uint32_r        StringView::to_hex32() const
{
    return ::to_hex32(data(), size());
}

uint64_r        StringView::to_hex64() const
{
    return ::to_hex64(data(), size());
}

int8_r          StringView::to_int8() const
{
    return ::to_int8(data(), size());
}

int16_r         StringView::to_int16() const
{
    return ::to_int16(data(), size());
}

int32_r         StringView::to_int32() const
{
    return ::to_int32(data(), size());
}

int64_r         StringView::to_int64() const
{
    return ::to_int64(data(), size());
}

integer_r      StringView::to_integer() const
{
    return ::to_integer(data(), size());
}

short_r         StringView::to_short() const
{
    return ::to_short(data(), size());
}

std::u8string   StringView::to_u8string() const
{
    return std::u8string((const char8_t*) data(), size());
}


u32string_r    StringView::to_u32string() const
{
    std::u32string    ret;
    ret.reserve(size());
    bool ok = for_each([&](char32_t c){
        ret += c;
    });
    return u32string_r(ok, ret);
}

uint8_r         StringView::to_uint8() const
{
    return ::to_uint8(data(), size());
}

uint16_r        StringView::to_uint16() const
{
    return ::to_uint16(data(), size());
}

uint32_r        StringView::to_uint32() const
{
    return ::to_uint32(data(), size());
}

uint64_r        StringView::to_uint64() const
{
    return ::to_uint64(data(), size());
}


unsigned_r      StringView::to_uinteger() const
{
    return ::to_uinteger(data(), size());
}

ushort_r  StringView::to_ushort() const
{
    return ::to_ushort(data(), size());
}

wstring_r    StringView::to_wstring() const
{
    std::wstring    ret;
    ret.reserve(size());
    
    bool ok = for_each_w([&](wchar_t c) {
        ret += c;
    });
    return wstring_r(ok, ret);
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


StringView::Iter32::Iter32(const char*z) : Iter32(z, strlen((const char*) z))
{
}


size_t      StringView::Iter32::position() const 
{ 
    return m_data - m_begin; 
}

Char32      StringView::Iter32::next()
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

StringView::IterW::IterW(const char*z, size_t n) : m_begin(z), m_data(z), m_end(z+n), m_state{}, m_error(false)
{
}

StringView::IterW::IterW(const std::string& s) : IterW(s.data(), s.size())
{
}

StringView::IterW::IterW(const std::string_view&s) : IterW(s.data(), s.size())
{
}

StringView::IterW::IterW(const char*z) : IterW(z, strlen((const char*) z))
{
}

size_t      StringView::IterW::position() const 
{ 
    return m_data - m_begin; 
}

wchar_t    StringView::IterW::next()
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
