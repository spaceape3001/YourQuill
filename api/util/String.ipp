////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////




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

int                 String::index_of(char ch) const
{
    auto i  = find_first_of(ch);
    return (i != npos) ? (int) i : -1;
}

int                 String::index_of(char ch, int j) const
{
    auto i  = find_first_of(ch, j);
    return (i != npos) ? (int) i : -1;
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

int             String::last_index_of(char ch) const
{
    auto i = find_last_of(ch);
    return (i != npos) ? (int) i : -1;
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

String  String::mid(int pos) const
{
    return substr(pos);
}

String  String::mid(int pos, int len) const
{
    return substr(pos, len);
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

String      operator+(const std::string&a, const std::string_view&b)
{
    String  ret(a);
    ret += b;
    return ret;
}

